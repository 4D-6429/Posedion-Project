#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ibus.h"
#include "motor.h"
#include "control.h"

#define IBUS_UART    UART_NUM_1
#define IBUS_RX_GPIO GPIO_NUM_16

/* 1 yapip yeniden yukle: normal kontrolun yerine ESC kalibrasyon dizisi calisir.
 * Kalibrasyon bitince tekrar 0 yapip yeniden yuklemeyi unutma. */
#define ESC_CALIBRATION_MODE  0

void app_main(void)
{
    ESP_ERROR_CHECK(ibus_start_task(IBUS_UART, IBUS_RX_GPIO));
    ESP_ERROR_CHECK(motor_init());

#if ESC_CALIBRATION_MODE
    motor_calibrate_escs();
#endif

    control_init();

    while (1) {
        control_update();
        vTaskDelay(pdMS_TO_TICKS(20));  /* 50Hz kontrol döngüsü */
    }
}
