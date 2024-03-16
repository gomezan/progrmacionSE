#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "driver/timer.h"

#define TIMER_INTERVAL_US 7000 // Timer interval in microseconds

#define GPIO_OUTPUT_IO_0    CONFIG_GPIO_OUTPUT_0
#define GPIO_OUTPUT_IO_1    CONFIG_GPIO_OUTPUT_1
#define GPIO_OUTPUT_IO_2    CONFIG_GPIO_OUTPUT_2
#define GPIO_OUTPUT_IO_3    CONFIG_GPIO_OUTPUT_3
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1) | (1ULL<<GPIO_OUTPUT_IO_2) | (1ULL<<GPIO_OUTPUT_IO_3))

void app_main(void)
{
    // Configure GPIO pins
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = GPIO_OUTPUT_PIN_SEL,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&io_conf);

    // Configure timer
    timer_config_t timer_config = {
        .alarm_en = TIMER_ALARM_DIS, // Disable alarm
        .counter_en = TIMER_START,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_DOWN, // Count down
        .auto_reload = TIMER_AUTORELOAD_DIS, // Disable autoreload
        .divider = 2 // 80 MHz / 2 = 40 MHz
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &timer_config);

    // Set initial counter value for 6.25 ms
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_US);

    int cnt = 0;
    uint64_t remaining_time = 0;
    uint64_t prev_result = 0;
    while (1) {
        // Check if the timer has expired
        prev_result = remaining_time;
        timer_get_counter_value(TIMER_GROUP_0, TIMER_0,&remaining_time);
        if (remaining_time != prev_result) {
            // Timer expired, do something
            printf("Timer expired!\n");

            // Reset counter value to 6.25 ms
            timer_set_counter_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_US);

            printf("cnt: %d\n", cnt++);
            gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
            gpio_set_level(GPIO_OUTPUT_IO_1, cnt % 2);
            gpio_set_level(GPIO_OUTPUT_IO_2, cnt % 2);
            gpio_set_level(GPIO_OUTPUT_IO_3, cnt % 2);
        }
    }
}
