#pragma once

#include "esp_err.h"

/* ── Mixing modu ───────────────────────────────────────────────────────────
 * CTRL_MIXING_SCALE_DOWN  : gaz ve yaw orantılı küçülür, yön korunur
 *                           otonom mod ve hassas manevra için önerilir
 * CTRL_MIXING_GAZ_PRIORITY: gaz korunur, yaw sığdığı kadar girer
 *                           tam gaz'da dönüş kısıtlanır */
#define CTRL_MIXING_SCALE_DOWN    0
#define CTRL_MIXING_GAZ_PRIORITY  1

#define CTRL_MIXING_MODE  CTRL_MIXING_SCALE_DOWN

/* ── Kontrol parametreleri ─────────────────────────────────────────────────
 * DEADZONE      : kumanda merkeze döndüğünde ±bu kadar sapma sıfır sayılır
 * MAX_THROTTLE  : maksimum ileri/geri hız  (0.0 = dur,      1.0 = tam gaz)
 * MAX_YAW       : maksimum dönüş hızı      (0.0 = dönme yok, 1.0 = tam)
 * RAMP_RATE     : her döngüde (20ms) motorun max değişim miktarı
 *                 0.02 → 0'dan 1'e 1.0s | 0.05 → 0.4s | 0.10 → 0.2s
 * SOFT_START_MS : boot sonrası ESC arm için nötrde bekleme (ms) */
#define CTRL_DEADZONE       0.05f
#define CTRL_MAX_THROTTLE   0.80f
#define CTRL_MAX_YAW        0.70f
#define CTRL_RAMP_RATE      0.02f
#define CTRL_SOFT_START_MS  2000

/* Soft start uygular: motorları nötrde tutar, ESC arm süresini bekler. */
void control_init(void);

/* Bir kontrol döngüsü çalıştırır: iBUS oku → deadzone/limit/mix/ramp → motor yaz.
 * 20ms'de bir çağrılmalı (50Hz). */
void control_update(void);
