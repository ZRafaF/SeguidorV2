#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "driver/adc.h"
#include "esp_adc_cal.h"


#include "esp_log.h"


#define SENS_NUM 11


/*
S2 39 1_3
S1 36 1_0
S3 34 1_6
S4 35 1_7
S5 32 1_4
S6 33 1_5
S7 25 2_8
S8 26 2_9
S9 27 2_7
S10 14 2_6
S11 13 2_4

  (S2, 2082), 0);
  (S1, 1702), 1);
  (S3, 1671), 2);
  (S4, 1707), 3);
  (S5, 2094), 4);
  (S11, 2095), 5);
  (S6, 2097), 6);
  (S10, 2095), 7);
  (S9, 2084), 8);
  (S7, 2084), 9);
  (S8, 2084), 10);
*/

/*
    S2 39 1_3
    S1 36 1_0
    S3 34 1_6
    S4 35 1_7
    S5 32 1_4
    S11 13 2_4
    S6 33 1_5
    S10 14 2_6
    S9 27 2_7
    S7 25 2_8
    S8 26 2_9
*/

typedef struct Sensor
{
    u_int16_t analogValue;
    u_int16_t middlePoint;
    bool digitalRead;

} Sensor;


static const adc_bits_width_t width = ADC_WIDTH_BIT_9;



uint16_t readAnalog(uint8_t sensorNumber)
{
    int reading = 0;
    switch (sensorNumber)
    {
    case 0:
        reading = adc1_get_raw(ADC1_CHANNEL_3);
        break;
    case 1:
        reading = adc1_get_raw(ADC1_CHANNEL_1);
        break;
    case 2:
        reading = adc1_get_raw(ADC1_CHANNEL_6);
        break;
    case 3:
        reading = adc1_get_raw(ADC1_CHANNEL_7);
        break;
    case 4:
        reading = adc1_get_raw(ADC1_CHANNEL_4);
        break;
    case 5:
        adc2_get_raw(ADC2_CHANNEL_4, width, &reading);
        break;
    case 6:
        reading = adc1_get_raw(ADC1_CHANNEL_5);
        break;
    case 7:
        adc2_get_raw(ADC2_CHANNEL_6, width, &reading);
        break;
    case 8:
        adc2_get_raw(ADC2_CHANNEL_7, width, &reading);
        break;
    case 9:
        adc2_get_raw(ADC2_CHANNEL_8, width, &reading);
        break;
    case 10:
        adc2_get_raw(ADC2_CHANNEL_9, width, &reading);
        break;
    default:
        return 0;
    }
    
    return reading;
}

void taskGetSensorInput(Sensor *sensArr)
{
    for(uint8_t i = 0; i < SENS_NUM; i++)
    {
        (sensArr + i) -> analogValue = readAnalog(i);

    }

    
}


void app_main()
{
    Sensor sensorArray[SENS_NUM];
    gpio_pad_select_gpio (GPIO_NUM_12);
    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);

    
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_6);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_2_5);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC2_CHANNEL_4, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC2_CHANNEL_6, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC2_CHANNEL_8, ADC_ATTEN_DB_6);
    adc2_config_channel_atten(ADC2_CHANNEL_9, ADC_ATTEN_DB_6);
    


    adc1_config_width(width);

    while (1)
    {

        taskGetSensorInput(sensorArray);
        for(uint8_t i = 0; i < SENS_NUM; i++)
        {
            printf("%i, ", sensorArray[i].analogValue);    
        }
        printf("\n");
    }
    

    while (true)
    {
        printf("bbb\n");
        gpio_set_level(GPIO_NUM_12, 1);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_12, 0);
        vTaskDelay(1000/portTICK_PERIOD_MS);

    }
}