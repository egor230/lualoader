#pragma once

// ============================================================================
// CMapCoordinates (23.11.2026) — преобразование точки карты меню в реальные
// координаты игрового мира Vice City (экран -> радар -> мир).
// ============================================================================

bool ma_map_crosshair_world(float& worldX, float& worldY); // false = меню карты не открыто
bool ma_map_uv_world(float u, float v, float& worldX, float& worldY); // точка нашего оверлея 0..1 -> мир
void ma_map_coords_diag();
void ma_map_diag_uv(float u, float v, float wx, float wy); // расширенная диагностика перекрестия                                 // лог полей CMenuManager
