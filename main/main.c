#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"

#define LDR_CHANNEL ADC1_CHANNEL_3   // GPIO34
#define BUZZER_GPIO 18

void pwm_init()
{
    // Timer configuratie
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    // Kanaal configuratie
    ledc_channel_config_t channel = {
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 20, // 50%
        .hpoint = 0
    };
    ledc_channel_config(&channel);
}

void app_main(void)
{
    // ADC setup (ESP-IDF v5: geen esp_adc_cal meer)
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(LDR_CHANNEL, ADC_ATTEN_DB_12);

    pwm_init();

    while (1)
    {
        // 1. Lees sensor
        int raw = adc1_get_raw(LDR_CHANNEL);

        // 2. Zet om naar spanning (0 - 3300 mV)
        int voltage = (raw * 3300) / 4095;

        printf("Licht: %d mV\n", voltage);

        // 3. Map naar frequentie (theremin effect)
        int freq = 200 + (voltage * 1800 / 3300);

        // 4. Zet PWM frequentie
        ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);

        // 5. 50% duty = beste toon voor piezo
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 20);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}