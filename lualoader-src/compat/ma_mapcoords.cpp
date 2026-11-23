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
    // КАЛИБРОВКА ПО НАСТОЯЩЕЙ КАРТЕ fronten2.txd (3x3 тайла).
    // Формула взята из обработчика мыши проекта GTA Vice City Interactive Map
    // (без смещений маркера 28.375 / 54, которые туда НЕ относятся):
    //   worldX = (pixelX - 478) * 3.91232329951
    //   worldY = (414 - pixelY) * 3.91232329951
    // u,v — доля по собранному изображению 900x720 (3 тайла по 300x240).
    const float imgW = 900.0f;
    const float imgH = 720.0f;
    const float scale = 3.91232329951f;
    float px = u * imgW;
    float py = v * imgH;
    worldX = (px - 478.0f) * scale;
    worldY = (414.0f - py) * scale;
    return true;
}
