#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

/* ESC PWM sinyal parametreleri: 50Hz, pulse 1000..2000µs.
 * 1000µs = tam geri / stop | 1500µs = nötr | 2000µs = tam ileri */
#define MOTOR_PULSE_MIN_US   1000
#define MOTOR_PULSE_MID_US   1500
#define MOTOR_PULSE_MAX_US   2000

/* Sol motor ESC'sinin gerçek nötr noktası 1500µs'den kaymışsa telafi için.
 * Pozitif değer nötrü/pulse'ı yukarı kaydırır. Donanımda test ederek ayarla. */
#define MOTOR_LEFT_TRIM_US   -13

/* TODO: Donanım bağlantısı netleşince bu pinleri güncelle */
#define MOTOR_LEFT_GPIO    GPIO_NUM_17   /* sol thruster ESC sinyal pini */
#define MOTOR_RIGHT_GPIO   GPIO_NUM_18   /* sağ thruster ESC sinyal pini */

/* MCPWM kaynakları: grup 0 */
#define MOTOR_MCPWM_GROUP  0

/* ─── API ──────────────────────────────────────────────────────────────────
 * Kullanım:
 *   motor_init();                     // app_main başında bir kez
 *   motor_set(gaz + yaw, gaz - yaw); // -1.0..1.0, clamp otomatik
 *   motor_stop();                     // failsafe veya kapanışta */

/* MCPWM timer + operator + comparator + GPIO init eder.
 * MOTOR_LEFT_GPIO ve MOTOR_RIGHT_GPIO GPIO_NUM_NC ise ESP_ERR_INVALID_ARG döner. */
esp_err_t motor_init(void);

/* Her iki motoru ayarlar. left/right: -1.0..1.0 (sınır dışı clamp edilir).
 * -1.0 → 1000µs, 0.0 → 1500µs, 1.0 → 2000µs */
void motor_set(float left, float right);

/* Her iki motoru nötral pulse (1500µs) olarak durdurur. */
void motor_stop(void);

/* ESC kalibrasyon dizisi: MAX → bekle → NÖTR → bekle, log üzerinden yönlendirir.
 * SADECE main.c içindeki ESC_CALIBRATION_MODE=1 iken çağrılır, geri dönmez.
 * DİKKAT: ESC'lere güç vermeden önce pervane/itki bağlı değilse güvenlidir. */
void motor_calibrate_escs(void);
