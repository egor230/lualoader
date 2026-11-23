#define _CRT_SECURE_NO_WARNINGS
// ============================================================================
// CMapOverlay — большая карта по клавише H + ПЕРЕКРЕСТИЕ из двух линий.
// Стрелки двигают перекрестие, в точке пересечения печатается World X/Y.
// BUILD-GATE: только POD/указатели, без глобальных конструкторов и TLS.
// ============================================================================
#include "ma_mapoverlay.h"
#include "../funcs.h"
#include "CTxdStore.h"
#include "RenderWare.h"
#include "CSprite2d.h"
#include "ma_mapcoords.h"

#include <windows.h>
#include <cstdio>

static CSprite2d* g_tiles = nullptr;
static bool       g_loaded  = false;
static bool       g_enabled = false;
static bool       g_prevH   = false;
static unsigned int g_lastDrawTime = 0;
static bool       g_diagDone = false;

// положение перекрестия в нормализованных координатах карты 0..1
static float g_curU = 0.5f, g_curV = 0.5f;

static const char* k_tileNames[9] = {
    "mapTop01", "mapTop02", "mapTop03",
    "mapMid01", "mapMid02", "mapMid03",
    "mapBot01", "mapBot02", "mapBot03"
};

void ma_map_cross_set(float u, float v) {
    if (u < 0.0f) u = 0.0f; if (u > 1.0f) u = 1.0f;
    if (v < 0.0f) v = 0.0f; if (v > 1.0f) v = 1.0f;
    g_curU = u; g_curV = v;
}
void ma_map_cross_get(float& u, float& v) { u = g_curU; v = g_curV; }

static void load_menu_map() {
    if (g_loaded) return;
    int slot = CTxdStore::AddTxdSlot("vc_menu_map");
    if (slot == -1 || !CTxdStore::LoadTxd(slot, "models/fronten2.txd")) {
        char dbg[160];
        _snprintf_s(dbg, sizeof(dbg), _TRUNCATE,
            "MAPOVERLAY: не загрузить fronten2.txd (slot=%d)", slot);
        writelog(dbg);
        g_loaded = true;
        return;
    }
    g_tiles = new CSprite2d[9];
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(slot);
    int ok = 0;
    for (int i = 0; i < 9; ++i) {
        g_tiles[i].SetTexture(const_cast<char*>(k_tileNames[i]));
        if (g_tiles[i].m_pTexture) ++ok;
    }
    CTxdStore::PopCurrentTxd();
    char dbg[192];
    _snprintf_s(dbg, sizeof(dbg), _TRUNCATE,
        "MAPOVERLAY: fronten2.txd загружен, слот=%d, текстур карты меню=%d/9", slot, ok);
    writelog(dbg);
    g_loaded = true;
}

void ma_mapoverlay_set(bool on) {
    if (on == g_enabled) return;
    g_enabled = on;
    char dbg[128];
    _snprintf_s(dbg, sizeof(dbg), _TRUNCATE, "MAPOVERLAY: Lua -> карта %s",
                g_enabled ? "ВКЛЮЧЕНА" : "ВЫКЛЮЧЕНА");
    writelog(dbg);
    if (g_enabled) load_menu_map();
}

bool ma_mapoverlay_is_on() { return g_enabled; }

void ma_mapoverlay_shutdown() {
    if (g_tiles) {
        for (int i = 0; i < 9; ++i) g_tiles[i].Delete();
        delete[] g_tiles;
        g_tiles = nullptr;
    }
    g_loaded = false;
    g_enabled = false;
    g_prevH = false;
}

void ma_mapoverlay_frame() {
    if (!g_diagDone) {
        g_diagDone = true;
        writelog("MAPOVERLAY: карта меню + перекрестие (стрелки)");
    }
    bool nowH = (GetAsyncKeyState(0x48) & 0x8000) != 0;
    if (nowH && !g_prevH) {
        g_enabled = !g_enabled;
        char dbg[128];
        _snprintf_s(dbg, sizeof(dbg), _TRUNCATE, "MAPOVERLAY: H — карта %s",
                    g_enabled ? "ВКЛЮЧЕНА" : "ВЫКЛЮЧЕНА");
        writelog(dbg);
        if (g_enabled) load_menu_map();
    }
    g_prevH = nowH;

    if (!g_enabled) return;

    // стрелки двигают перекрестие (плавно при удержании)
    const float step = 0.0035f;
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000) g_curU -= step;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) g_curU += step;
    if (GetAsyncKeyState(VK_UP)    & 0x8000) g_curV -= step;
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000) g_curV += step;
    if (g_curU < 0.0f) g_curU = 0.0f; if (g_curU > 1.0f) g_curU = 1.0f;
    if (g_curV < 0.0f) g_curV = 0.0f; if (g_curV > 1.0f) g_curV = 1.0f;

    // N — показать координаты точки пересечения штатным сообщением игры
    static bool prevN = false;
    bool nowN = (GetAsyncKeyState(0x4E) & 0x8000) != 0;  // 0x4E = 'N'
    if (nowN && !prevN) {
        float wx, wy;
        if (ma_map_uv_world(g_curU, g_curV, wx, wy)) {
            wchar_t msg[96];
            swprintf(msg, 96, L"~y~Map point: X %.1f  Y %.1f", wx, wy);
            CMessages::AddMessageJumpQ(msg, 2000, 1);
        } else {
            CMessages::AddMessageJumpQ(L"~r~Map coordinates unavailable", 2000, 1);
        }
    }
    prevN = nowN;
}

static void draw_map() {
    if (!g_enabled || !g_loaded || !g_tiles) return;
    unsigned int now = CTimer::m_snTimeInMilliseconds;
    if (now == g_lastDrawTime) return;   // защита от двойного вызова drawBlipsEvent
    g_lastDrawTime = now;

    float W = SCREEN_WIDTH, H = SCREEN_HEIGHT;
    float size = (W < H ? W : H) * 0.92f;
    float left = (W - size) * 0.5f;
    float top  = (H - size) * 0.5f;
    float tile = size / 3.0f;

    CSprite2d::DrawRect(CRect(left - 6, top - 6, left + size + 6, top + size + 6),
                        CRGBA(0, 0, 0, 200));
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int i = row * 3 + col;
            if (!g_tiles[i].m_pTexture) continue;
            float x0 = left + col * tile, y0 = top + row * tile;
            g_tiles[i].Draw(CRect(x0, y0, x0 + tile, y0 + tile), CRGBA(255, 255, 255, 255));
        }
    }

    // === ПЕРЕКРЕСТИЕ: две линии ===
    float cx = left + g_curU * size;
    float cy = top  + g_curV * size;
    CSprite2d::DrawRect(CRect(left, cy - 1, left + size, cy + 1), CRGBA(255, 40, 40, 220)); // горизонталь
    CSprite2d::DrawRect(CRect(cx - 1, top, cx + 1, top + size), CRGBA(255, 40, 40, 220)); // вертикаль
    CSprite2d::DrawRect(CRect(cx - 4, cy - 4, cx + 4, cy + 4), CRGBA(255, 255, 0, 255));  // точка

    // Никакого самодельного текста и прямоугольника (по просьбе пользователя).
    // Координаты показываем штатным игровым сообщением по клавише N — как в taxi.cs.
}

void ma_mapoverlay_install() {
    Events::initRwEvent += [] { load_menu_map(); };
    Events::drawBlipsEvent += [] { draw_map(); };
    Events::shutdownRwEvent += [] { ma_mapoverlay_shutdown(); };
}
