#include "control.h"
#include "ibus.h"
#include "motor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define IBUS_UART  UART_NUM_1

static const char *TAG = "control";

static float s_out_left  = 0.0f;
static float s_out_right = 0.0f;

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static float ramp_toward(float current, float target)
{
    if (target > current + CTRL_RAMP_RATE) return current + CTRL_RAMP_RATE;
    if (target < current - CTRL_RAMP_RATE) return current - CTRL_RAMP_RATE;
    return target;
}

void control_init(void)
{
    ESP_LOGI(TAG, "Soft start: %d ms bekleniyor...", CTRL_SOFT_START_MS);
    motor_stop();
    vTaskDelay(pdMS_TO_TICKS(CTRL_SOFT_START_MS));
    ESP_LOGI(TAG, "Hazir");
}

void control_update(void)
{
    if (ibus_is_failsafe(IBUS_UART)) {
        ESP_LOGW(TAG, "FAILSAFE — motorlar durduruluyor");
        s_out_left  = 0.0f;
        s_out_right = 0.0f;
        motor_stop();
        return;
    }

    float gaz = ibus_get_normalized(2);   /* CH3 */
    float yaw = ibus_get_normalized(3);   /* CH4 */

    /* deadzone: merkezdeki titremeleri sıfırla */
    if (gaz > -CTRL_DEADZONE && gaz < CTRL_DEADZONE) gaz = 0.0f;
    if (yaw > -CTRL_DEADZONE && yaw < CTRL_DEADZONE) yaw = 0.0f;

    /* hız ve dönüş limitleri */
    gaz = clampf(gaz, -CTRL_MAX_THROTTLE, CTRL_MAX_THROTTLE);
    yaw = clampf(yaw, -CTRL_MAX_YAW,      CTRL_MAX_YAW);

    float target_left, target_right;

#if CTRL_MIXING_MODE == CTRL_MIXING_SCALE_DOWN
    /* gaz ve yaw orantılı küçülür, yön korunur */
    target_left  = gaz + yaw;
    target_right = gaz - yaw;
    float maxval = target_left  >  target_right  ? target_left  :
                   target_right >  0             ? target_right :
                  -target_left  > -target_right  ? -target_left : -target_right;
    if (maxval > 1.0f) { target_left /= maxval; target_right /= maxval; }

#elif CTRL_MIXING_MODE == CTRL_MIXING_GAZ_PRIORITY
    /* gaz korunur, yaw sığdığı kadar girer */
    float max_yaw_room = 1.0f - (gaz > 0 ? gaz : -gaz);
    float yaw_eff = clampf(yaw, -max_yaw_room, max_yaw_room);
    target_left  = gaz + yaw_eff;
    target_right = gaz - yaw_eff;

#else
#error "Gecersiz CTRL_MIXING_MODE — control.h dosyasini kontrol et"
#endif

    /* ramp: ani değişim yerine kademeli yaklaşım */
    s_out_left  = ramp_toward(s_out_left,  target_left);
    s_out_right = ramp_toward(s_out_right, target_right);

    motor_set(s_out_left, s_out_right);

    ESP_LOGI(TAG, "gaz=%.2f yaw=%.2f | L=%.2f R=%.2f", gaz, yaw, s_out_left, s_out_right);
}
