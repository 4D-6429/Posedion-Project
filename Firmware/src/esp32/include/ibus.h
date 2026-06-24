#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_err.h"

/* iBUS protokolü: 115200 baud, 8N1, 32 byte frame, her ~7ms bir frame gelir.
 * Frame yapısı: [0x20][0x40][ch1_lo][ch1_hi]...[ch14_lo][ch14_hi][csum_lo][csum_hi]
 * Her kanal: 1000 (min) .. 1500 (merkez) .. 2000 (max), uint16 little-endian. */

#define IBUS_NUM_CHANNELS      14
#define IBUS_CH_MIN            1000
#define IBUS_CH_MID            1500
#define IBUS_CH_MAX            2000
#define IBUS_FAILSAFE_MS       200  /* bu süre valid frame gelmezse failsafe */

typedef struct {
    uint16_t channel[IBUS_NUM_CHANNELS];  /* channel[0] = CH1 .. channel[13] = CH14 */
} ibus_data_t;

/* ─── Low-level API ────────────────────────────────────────────────────────────
 * Okuma ve zamanlama kontrolünü kendin yapacaksan kullan. */

/* UART'ı iBUS için yapılandırır. ibus_start_task() kullanıyorsan çağırma. */
esp_err_t ibus_init(uart_port_t uart_num, gpio_num_t rx_pin);

/* Tek bir geçerli frame okur. timeout_ms içinde frame gelmezse false döner.
 * Checksum yanlışsa atlar, doğru frame bulana kadar tarar. Bloklayıcıdır. */
bool ibus_read(uart_port_t uart_num, ibus_data_t *out, uint32_t timeout_ms);

/* ibus_read() sonucu üzerinden kanal değeri okur. ch: 0..13 arası indeks. */
uint16_t ibus_channel(const ibus_data_t *data, uint8_t ch);

/* Kanal değerini -1.0 .. 1.0 aralığına normalize eder.
 * 1000 → -1.0 | 1500 → 0.0 | 2000 → 1.0. Sınır dışı değerler clamp edilir. */
float ibus_normalize(const ibus_data_t *data, uint8_t ch);

/* Son geçerli frame'den bu yana IBUS_FAILSAFE_MS geçtiyse true döner.
 * Verici kapandı / menzil dışı durumunu tespit eder. */
bool ibus_is_failsafe(uart_port_t uart_num);


/* ─── Task-based API ───────────────────────────────────────────────────────────
 * Arka planda FreeRTOS task çalışır, son frame'i depolar.
 * ibus_get() / ibus_get_normalized() bloklamamaz, herhangi bir task'tan çağrılabilir.
 *
 * Kullanım:
 *   ibus_start_task(UART_NUM_1, GPIO_NUM_16);   // başlangıçta bir kez
 *   uint16_t gaz = ibus_get(2);                 // CH3 = gaz (0 indeksli)
 *   float dir   = ibus_get_normalized(3);        // CH4 = -1.0..1.0 */

/* UART init + arka plan task başlatır. ibus_init() yerine bunu çağır. */
esp_err_t ibus_start_task(uart_port_t uart_num, gpio_num_t rx_pin);

/* Son alınan frame'den kanal değeri döner (1000..2000). Thread-safe. */
uint16_t ibus_get(uint8_t ch);

/* Son alınan frame'den normalize kanal değeri döner (-1.0..1.0). Thread-safe. */
float ibus_get_normalized(uint8_t ch);
