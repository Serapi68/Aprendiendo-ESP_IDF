#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2
#define LED2_PIN GPIO_NUM_4

void binario(void *pvParameter)
{

    while (1)
    {
        // Primer tiempo
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(300));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(300));
        // Segundo tiempo
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(300));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void ternario (void *pvParameter)
{

    while (1)
    {
        //Primer tiempo
        gpio_set_level(LED2_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(200)); //Primer tresillo - Suena
        gpio_set_level(LED2_PIN, 0); 
        vTaskDelay(pdMS_TO_TICKS(200)); //Segundo tresillo -No suena
        gpio_set_level(LED2_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(200)); //Tercer tresillo - Suena
        //segundo tiempo
        gpio_set_level(LED2_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(200)); // Cuarto tresillo - No suena
        gpio_set_level(LED2_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(200)); //Quinto tresillo - Suena
        gpio_set_level(LED2_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(200)); // Sexto tresillo - No suena
    }
}

//Creamos las tareas;xTaskCreate(EL VOID, "Nombre", Stack, Parametro, Prioridad 1/24, handle/null);
void app_main(void)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);

    xTaskCreate(binario, "Primero", 2048, NULL, 5, NULL);
    xTaskCreate(ternario, "Segundo", 2048, NULL, 5, NULL);

}