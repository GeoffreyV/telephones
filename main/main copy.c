/*
#include <stdio.h>
#include "driver/mcpwm.h"
#include "driver/mcpwm_gen.h"
#include "driver/gpio.h"

#define GPIO_PWM0A_OUT 13   //Set GPIO 18 as PWM0A
#define GPIO_PWM0B_OUT 9   //Set GPIO 19 as PWM0B
#define GPIO_OUTPUT_IO_18    18

void mcpwm_example_gpio_initialize()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT);
}

void app_main(void)
{

    gpio_set_direction(GPIO_OUTPUT_IO_18, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_OUTPUT_IO_18, 1);

    printf("Testing MCPWM...\n");
    mcpwm_example_gpio_initialize();

    mcpwm_timer_set_resolution(MCPWM_UNIT_0, MCPWM_TIMER_0, 1000); //Set timer period of PWM0A & PWM0B  // 1ms (1KHz


    //MCPWM configuration 
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 5;    //frequency = 1000Hz
    pwm_config.cmpr_a = 50.0;       //duty cycle of PWMxA = 50.0%
    pwm_config.cmpr_b = 50.0;       //duty cycle of PWMxb = 50.0%
    pwm_config.counter_mode = MCPWM_UP_DOWN_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
    mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE, 100, 100);

}*/