#pragma once

// ============================================================================
// CMapOverlay (23.11.2026) — большая карта Vice City поверх игры по клавише H.
// Текстура берётся из игрового models/MAP.txd (имя "radar"), рисуется через
// CSprite2d в Events::drawingEvent. Маркер Томми и спрятанные пакеты — слои.
// ============================================================================

void ma_mapoverlay_install();   // подписка на события ( вызывается из Message )
void ma_mapoverlay_frame();     // опрос клавиши H + сбор данных ( игровой поток )
void ma_mapoverlay_shutdown();  // освобождение спрайта
void ma_mapoverlay_set(bool on);     // показать/скрыть карту из Lua
bool ma_mapoverlay_is_on();          // статус карты
void ma_map_cross_set(float u, float v); // положение перекрестия 0..1
void ma_map_cross_get(float& u, float& v);
