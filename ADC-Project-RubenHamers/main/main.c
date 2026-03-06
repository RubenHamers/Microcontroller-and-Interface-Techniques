#include <stdio.h>

#include "analoog.h"

#include "driver/gpio.h"
#include "driver/adc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define LED GPIO_NUM_2

#define LDR_CHANNEL ADC_CHANNEL_6   // bv GPIO34
#define POT_CHANNEL ADC_CHANNEL_7   // bv GPIO35

void app_main(void)
{
    // ADC kanalen instellen
    myADC_setup(LDR_CHANNEL);
    myADC_setup(POT_CHANNEL);

    // LED instellen
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    while (1)
    {
        int licht = myADC_getValue(LDR_CHANNEL);
        int grens = myADC_getValue(POT_CHANNEL);

        printf("licht = %d   grens = %d\n", licht, grens);

        if (licht < grens)
        {
            gpio_set_level(LED, 1); // LED aan
        }
        else
        {
            gpio_set_level(LED, 0); // LED uit
        }

vTaskDelay(pdMS_TO_TICKS(200));
    }
}