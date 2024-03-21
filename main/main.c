#include <stdio.h>
#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"

#define GPIO_PWM0A_OUT 13   //Set GPIO 18 as PWM0A
#define GPIO_PWM0B_OUT 9   //Set GPIO 19 as PWM0B
#define GPIO_OUTPUT_IO_18    18


#define MCPWM_GROUP_ID 0
#define MCPWM_TIMER_0 0
#define MCPWM_TIMER_RESOLUTION 300000


mcpwm_timer_handle_t mcpwm_timer;
mcpwm_oper_handle_t mcpwm_operator;
mcpwm_gen_handle_t mcpwm_generators[2];
mcpwm_cmpr_handle_t mcpwm_comparator;
mcpwm_cap_timer_handle_t mcpwm_capture_timer;

static void gen_action_config(mcpwm_gen_handle_t gena, mcpwm_gen_handle_t genb, mcpwm_cmpr_handle_t cmpa, mcpwm_cmpr_handle_t cmpb)
{
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(gena,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(gena,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, cmpa, MCPWM_GEN_ACTION_LOW)));
}

static void dead_time_config(mcpwm_gen_handle_t gena, mcpwm_gen_handle_t genb)
{
    mcpwm_dead_time_config_t dead_time_config = {
        .posedge_delay_ticks = 10,
        .negedge_delay_ticks = 0
    };
    ESP_ERROR_CHECK(mcpwm_generator_set_dead_time(gena, gena, &dead_time_config));
    dead_time_config.posedge_delay_ticks = 0;
    dead_time_config.negedge_delay_ticks = 10;
    dead_time_config.flags.invert_output = true;
    ESP_ERROR_CHECK(mcpwm_generator_set_dead_time(gena, genb, &dead_time_config));
}   


void app_main(void)
{

    gpio_num_t pin_L=GPIO_NUM_13;
    gpio_num_t pin_H=GPIO_NUM_9;

    gpio_num_t gen_gpios[2] = {pin_L, pin_H};

    uint32_t frequency = 5;

    // Timer configuration and allocation
    uint32_t period_ticks  = MCPWM_TIMER_RESOLUTION / frequency;

    mcpwm_timer_config_t timer_config;
    timer_config.group_id = MCPWM_GROUP_ID;
    timer_config.clk_src = MCPWM_TIMER_0;
    timer_config.resolution_hz = MCPWM_TIMER_RESOLUTION;
    timer_config.count_mode = MCPWM_TIMER_COUNT_MODE_UP;
    timer_config.period_ticks = period_ticks;
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &mcpwm_timer));


    // Operator configuration and allocation
    mcpwm_operator_config_t operator_config = {
        .group_id = MCPWM_GROUP_ID,
        .intr_priority = 1,
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &mcpwm_operator));

    mcpwm_comparator_config_t comparator_config = {
        .intr_priority = 0
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(mcpwm_operator, &comparator_config, &mcpwm_comparator));

   

    // Generator configuration and allocation
    mcpwm_generator_config_t generator_config = {
        .flags = {
            .invert_pwm = false, 
            .io_loop_back = false, 
            .io_od_mode = false,
            .pull_down = false, 
            .pull_up = false
        }
    };
    for (int i = 0; i < 2; i++) {
        generator_config.gen_gpio_num = gen_gpios[i];
        ESP_ERROR_CHECK(mcpwm_new_generator(mcpwm_operator, &generator_config, &mcpwm_generators[i]));
    }

    // Enable and start timer
    ESP_ERROR_CHECK(mcpwm_timer_enable(mcpwm_timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(mcpwm_timer, MCPWM_TIMER_START_NO_STOP));

    // Timer to operator connection
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(mcpwm_operator, mcpwm_timer));

    gen_action_config(mcpwm_generators[0], mcpwm_generators[1], mcpwm_comparator, mcpwm_comparator);
    dead_time_config(mcpwm_generators[0], mcpwm_generators[1]);

     // Set the duty cycle to 50%
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(mcpwm_comparator, period_ticks / 2));

}