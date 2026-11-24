#define _CRT_SECURE_NO_WARNINGS
// ============================================================================
// CMapCoordinates — точное преобразование из Waypoint Mod (IslamMachido)
// + поля CMenuManager из Plugin-SDK. Без глобальных конструкторов/TLS.
// ============================================================================
#include "ma_mapcoords.h"
#include "../funcs.h"
#include "CMenuManager.h"
#include "CRadar.h"
#include "CVector2D.h"

#include <cmath>
#include <cstdio>

static inline float rd(float* p) { return *p; }

void ma_map_coords_diag() {
    CMenuManager& menu = FrontEndMenuManager;
    char dbg[320];
    _snprintf_s(dbg, sizeof(dbg), _TRUNCATE,
        "MAPCOORDS: drawMap=%d mouseTemp=(%d %d) zoom=%.5f base=(%.3f %.3f) "
        "k=(%.6f %.6f) b=(%.6f %.6f) s=(%.6f %.6f) screen=%dx%d",
        menu.m_bDrawRadarOrMap ? 1 : 0,
        menu.m_nMouseTempPosX, menu.m_nMouseTempPosY,
        menu.m_fMapZoom, menu.m_fMapBaseX, menu.m_fMapBaseY,
        rd((float*)0x68FD0C), rd((float*)0x68FD18),
        rd((float*)0x68FD10), rd((float*)0x68FD1C),
        rd((float*)0x68FD14), rd((float*)0x68FD20),
        (int)RsGlobal.screenWidth, (int)RsGlobal.screenHeight);
    writelog(dbg);
}

static bool screen_to_world(float sx, float sy, float& worldX, float& worldY) {
    CMenuManager& menu = FrontEndMenuManager;
    const float zoom = menu.m_fMapZoom;
    if (std::fabs(zoom) < 0.000001f) return false;

    const float kx = rd((float*)0x68FD0C), bx = rd((float*)0x68FD10), sx_ = rd((float*)0x68FD14);
    const float ky = rd((float*)0x68FD18), by = rd((float*)0x68FD1C), sy_ = rd((float*)0x68FD20);

    const float scaleX = (float)RsGlobal.screenWidth  * sx_;
    const float scaleY = (float)RsGlobal.screenHeight * sy_;

    if (std::fabs(scaleX) < 0.000001f || std::fabs(scaleY) < 0.000001f ||
        std::fabs(kx * zoom) < 0.000001f || std::fabs(ky * zoom) < 0.000001f)
        return false;

    CVector2D radarPoint;
    radarPoint.x = (sx / scaleX - (bx * zoom + menu.m_fMapBaseX)) / (kx * zoom);
    radarPoint.y = (menu.m_fMapBaseY - by * zoom - sy / scaleY) / (ky * zoom);

    CVector2D worldPoint;
    CRadar::TransformRadarPointToRealWorldSpace(worldPoint, radarPoint);
    if (!std::isfinite(worldPoint.x) || !std::isfinite(worldPoint.y)) return false;
    worldX = worldPoint.x; worldY = worldPoint.y;
    return true;
}

bool ma_map_crosshair_world(float& worldX, float& worldY) {
    // НЕ требуем m_bDrawRadarOrMap: наш оверлей рисует карту сам.
    return screen_to_world((float)FrontEndMenuManager.m_nMouseTempPosX,
                           (float)FrontEndMenuManager.m_nMouseTempPosY, worldX, worldY);
}

bool ma_map_uv_world(float u, float v, float& worldX, float& worldY) {
    // КАЛИБРОВКА ПО РЕАЛЬНОЙ КАРТЕ fronten2.txd (диагностика 24.11.2026):
    //  - 9 тайлов по 256x256 => собранная карта 768x768 (КВАДРАТ, не 900x720);
    //  - оверлей рисует её квадратом (size x size), значит u,v — доли квадрата;
    //  - границы мира Vice City (VC-MP): X -2350..1550, Y -1950..1950 (по 3900);
    //  - верх карты = север (+Y), низ = юг (-Y): Ocean Beach (-1224,-1200) внизу,
    //    Downtown (-680,1000) наверху — направление Y подтверждено.
    // Линейное преобразование (масштаб 3900 ед на всю карту):
    // Пробная аффинная модель (TODO: заменить coefficients после сбора 8-10 точек).
    // Сейчас оставлена прежняя линейная формула БЕЗ «временных» поправок —
    // пользователь запретил искусственные константы, нужен замер u,v в логе.
    worldX = -2350.0f + u * 3900.0f;
    worldY =  1950.0f - v * 3900.0f;
    return true;
}

// Расширенная диагностика (раздел 7 ТЗ): этапы А (U/V), Б (преобразование), В (blip).
void ma_map_diag_uv(float u, float v, float wx, float wy) {
    float W = SCREEN_WIDTH, H = SCREEN_HEIGHT;
    float size = (W < H ? W : H) * 0.92f;          // как в draw_map()
    float left = (W - size) * 0.5f, top = (H - size) * 0.5f;
    float crossX = left + u * size, crossY = top + v * size;  // экранный пиксель перекрестия
    const float srcW = 768.0f, srcH = 768.0f;      // 9 тайлов по 256x256
    float px = u * srcW, py = v * srcH;             // координата исходного изображения
    char dbg[512];
    _snprintf_s(dbg, sizeof(dbg), _TRUNCATE,
        "MAPDIAG [A] u=%.4f v=%.4f g_curU/V=(%.4f %.4f) screen=%.0fx%.0f mapLeft/Top=(%.1f %.1f) mapSize=%.1f crossScreen=(%.1f %.1f)",
        u, v, u, v, W, H, left, top, size, crossX, crossY);
    writelog(dbg);
    _snprintf_s(dbg, sizeof(dbg), _TRUNCATE,
        "MAPDIAG [B] source=768x768 px=%.2f py=%.2f -> worldX=%.1f worldY=%.1f",
        px, py, wx, wy);
    writelog(dbg);
}
