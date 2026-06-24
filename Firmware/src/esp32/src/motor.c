#include "motor.h"
#include "driver/mcpwm_prelude.h"
#include "esp_log.h"

static const char *TAG = "motor";

static mcpwm_cmpr_handle_t s_cmpr_left  = NULL;
static mcpwm_cmpr_handle_t s_cmpr_right = NULL;

/* pulse_us hesapla: -1.0..1.0 → MOTOR_PULSE_MIN_US..MOTOR_PULSE_MAX_US */
static uint32_t to_pulse_us(float val)
{
    if (val >  1.0f) val =  1.0f;
    if (val < -1.0f) val = -1.0f;
    if (val >= 0.0f)
        return (uint32_t)(MOTOR_PULSE_MID_US + val * (MOTOR_PULSE_MAX_US - MOTOR_PULSE_MID_US));
    else
        return (uint32_t)(MOTOR_PULSE_MID_US + val * (MOTOR_PULSE_MID_US - MOTOR_PULSE_MIN_US));
}

esp_err_t motor_init(void)
{
    if (MOTOR_LEFT_GPIO == GPIO_NUM_NC || MOTOR_RIGHT_GPIO == GPIO_NUM_NC) {
        ESP_LOGE(TAG, "Pin tanimsiz (GPIO_NUM_NC). motor.h dosyasindaki pinleri guncelle.");
        return ESP_ERR_INVALID_ARG;
    }

    /* Timer: 1MHz çözünürlük, 20ms periyot → 50Hz */
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_cfg = {
        .group_id      = MOTOR_MCPWM_GROUP,
        .clk_src       = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,   /* 1MHz → 1µs çözünürlük */
        .count_mode    = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks  = 20000,     /* 20000µs = 20ms = 50Hz */
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_cfg, &timer));

    /* Sol motor: operator + comparator + generator */
    mcpwm_oper_handle_t oper_left = NULL;
    mcpwm_operator_config_t oper_cfg = { .group_id = MOTOR_MCPWM_GROUP };
    ESP_ERROR_CHECK(mcpwm_new_operator(&oper_cfg, &oper_left));
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper_left, timer));

    mcpwm_comparator_config_t cmpr_cfg = { .flags.update_cmp_on_tez = true };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper_left, &cmpr_cfg, &s_cmpr_left));
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(s_cmpr_left, MOTOR_PULSE_MID_US));

    mcpwm_gen_handle_t gen_left = NULL;
    mcpwm_generator_config_t gen_left_cfg = { .gen_gpio_num = MOTOR_LEFT_GPIO };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper_left, &gen_left_cfg, &gen_left));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(gen_left,
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(gen_left,
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, s_cmpr_left, MCPWM_GEN_ACTION_LOW)));

    /* Sağ motor: operator + comparator + generator */
    mcpwm_oper_handle_t oper_right = NULL;
    ESP_ERROR_CHECK(mcpwm_new_operator(&oper_cfg, &oper_right));
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper_right, timer));

    ESP_ERROR_CHECK(mcpwm_new_comparator(oper_right, &cmpr_cfg, &s_cmpr_right));
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(s_cmpr_right, MOTOR_PULSE_MID_US));

    mcpwm_gen_handle_t gen_right = NULL;
    mcpwm_generator_config_t gen_right_cfg = { .gen_gpio_num = MOTOR_RIGHT_GPIO };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper_right, &gen_right_cfg, &gen_right));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(gen_right,
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(gen_right,
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, s_cmpr_right, MCPWM_GEN_ACTION_LOW)));

    /* Timer başlat */
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));

    ESP_LOGI(TAG, "Motor init OK — LEFT:GPIO%d RIGHT:GPIO%d", MOTOR_LEFT_GPIO, MOTOR_RIGHT_GPIO);
    return ESP_OK;
}

void motor_set(float left, float right)
{
    if (!s_cmpr_left || !s_cmpr_right) return;
    mcpwm_comparator_set_compare_value(s_cmpr_left,  to_pulse_us(left));
    mcpwm_comparator_set_compare_value(s_cmpr_right, to_pulse_us(right));
}

void motor_stop(void)
{
    if (!s_cmpr_left || !s_cmpr_right) return;
    mcpwm_comparator_set_compare_value(s_cmpr_left,  MOTOR_PULSE_MID_US);
    mcpwm_comparator_set_compare_value(s_cmpr_right, MOTOR_PULSE_MID_US);
}
