#include "ibus.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *IBUS_TAG = "ibus";

static ibus_data_t        s_shared;
static portMUX_TYPE       s_mux = portMUX_INITIALIZER_UNLOCKED;
static uart_port_t        s_task_uart;

#define IBUS_BAUD           115200
#define IBUS_FRAME_LEN      32
#define IBUS_RX_BUF         256
#define IBUS_HDR0           0x20
#define IBUS_HDR1           0x40
#define IBUS_UART_MAX       3

static TickType_t s_last_frame[IBUS_UART_MAX];

esp_err_t ibus_init(uart_port_t uart_num, gpio_num_t rx_pin)
{
    const uart_config_t cfg = {
        .baud_rate  = IBUS_BAUD,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    esp_err_t ret = uart_param_config(uart_num, &cfg);
    if (ret != ESP_OK) return ret;

    ret = uart_set_pin(uart_num,
                       UART_PIN_NO_CHANGE, rx_pin,
                       UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (ret != ESP_OK) return ret;

    ret = uart_driver_install(uart_num, IBUS_RX_BUF, 0, 0, NULL, 0);
    if (ret != ESP_OK) return ret;

    gpio_set_pull_mode(rx_pin, GPIO_PULLUP_ONLY);
    return ESP_OK;
}

bool ibus_read(uart_port_t uart_num, ibus_data_t *out, uint32_t timeout_ms)
{
    TickType_t deadline = xTaskGetTickCount() + pdMS_TO_TICKS(timeout_ms);
    uint8_t frame[IBUS_FRAME_LEN];

    while (xTaskGetTickCount() < deadline) {
        TickType_t left = deadline - xTaskGetTickCount();

        /* Scan for first header byte */
        uint8_t b;
        if (uart_read_bytes(uart_num, &b, 1, left) <= 0) return false;
        if (b != IBUS_HDR0) continue;

        /* Confirm second header byte */
        left = deadline - xTaskGetTickCount();
        if (uart_read_bytes(uart_num, &b, 1, left) <= 0) return false;
        if (b != IBUS_HDR1) continue;

        /* Read remaining 30 bytes */
        frame[0] = IBUS_HDR0;
        frame[1] = IBUS_HDR1;
        left = deadline - xTaskGetTickCount();
        int n = uart_read_bytes(uart_num, &frame[2], IBUS_FRAME_LEN - 2, left);
        if (n != IBUS_FRAME_LEN - 2) return false;

        /* Verify checksum: 0xFFFF - sum(byte[0..29]) == byte[30..31] LE */
        uint16_t sum = 0xFFFF;
        for (int i = 0; i < IBUS_FRAME_LEN - 2; i++) sum -= frame[i];
        uint16_t csum = (uint16_t)frame[30] | ((uint16_t)frame[31] << 8);
        if (sum != csum) continue;

        /* Parse channels */
        for (int i = 0; i < IBUS_NUM_CHANNELS; i++) {
            out->channel[i] = (uint16_t)frame[2 + i * 2]
                            | ((uint16_t)frame[3 + i * 2] << 8);
        }
        if (uart_num < IBUS_UART_MAX)
            s_last_frame[uart_num] = xTaskGetTickCount();
        return true;
    }

    return false;
}

float ibus_normalize(const ibus_data_t *data, uint8_t ch)
{
    if (ch >= IBUS_NUM_CHANNELS) return 0.0f;
    float v = ((float)data->channel[ch] - 1500.0f) / 500.0f;
    if (v < -1.0f) v = -1.0f;
    if (v >  1.0f) v =  1.0f;
    return v;
}

bool ibus_is_failsafe(uart_port_t uart_num)
{
    if (uart_num >= IBUS_UART_MAX) return true;
    return (xTaskGetTickCount() - s_last_frame[uart_num])
           > pdMS_TO_TICKS(IBUS_FAILSAFE_MS);
}

uint16_t ibus_channel(const ibus_data_t *data, uint8_t ch)
{
    if (ch >= IBUS_NUM_CHANNELS) return 0;
    return data->channel[ch];
}

static void ibus_task_fn(void *arg)
{
    ibus_data_t frame;
    while (1) {
        if (ibus_read(s_task_uart, &frame, 100)) {
            portENTER_CRITICAL(&s_mux);
            s_shared = frame;
            portEXIT_CRITICAL(&s_mux);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

esp_err_t ibus_start_task(uart_port_t uart_num, gpio_num_t rx_pin)
{
    esp_err_t ret = ibus_init(uart_num, rx_pin);
    if (ret != ESP_OK) return ret;
    s_task_uart = uart_num;
    BaseType_t ok = xTaskCreate(ibus_task_fn, "ibus", 4096, NULL, 5, NULL);
    if (ok != pdPASS) {
        ESP_LOGE(IBUS_TAG, "task create failed");
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

uint16_t ibus_get(uint8_t ch)
{
    if (ch >= IBUS_NUM_CHANNELS) return 0;
    portENTER_CRITICAL(&s_mux);
    uint16_t v = s_shared.channel[ch];
    portEXIT_CRITICAL(&s_mux);
    return v;
}

float ibus_get_normalized(uint8_t ch)
{
    float v = ((float)ibus_get(ch) - 1500.0f) / 500.0f;
    if (v < -1.0f) v = -1.0f;
    if (v >  1.0f) v =  1.0f;
    return v;
}
