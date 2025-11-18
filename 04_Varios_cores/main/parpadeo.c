#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

//Tarea que se ejecutara cuando reciba la señal de la cola de contador
void parpadeo_led(void *pvParameter)
{
    QueueHandle_t cola = (QueueHandle_t) pvParameter;
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    int valor_recibido = 0;

    while (1){
        //Esperando a recibir un valor de la cola
        if (xQueueReceive(cola, &valor_recibido, pdMS_TO_TICKS(10)) == pdTRUE){
            //Parpadeo del LED segun el valor recibido
            for (int i = 0; i < 5; i++){
                gpio_set_level(LED_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(LED_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
        //Asegurarse de que el LED este apagado cuando no se recibe nada
        gpio_set_level(LED_PIN, 1);

    }

}

void inicia_parpadeo(QueueHandle_t cola)
{
    xTaskCreatePinnedToCore(
        parpadeo_led,          // Funcion de la tarea
        "parpadeo_led",       // Nombre de task
        2048,                 // Tamaño de stack
        (void *) cola,       //  Parámetro de tarea
        5,                    // Prioridad de la tarea
        NULL,                 // Puntero a la tarea creada
        0);                   // Core 0
}