#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Se crea un contador que enviara su valor a una cola cada 5 incrementos
void contador(void *pvParameter)
{
    QueueHandle_t cola = (QueueHandle_t) pvParameter;
    int contador = 0;

    printf("Tarea contador iniciada en core %d\n", xPortGetCoreID());

    while(1){
        contador++;
        printf("Contador: %d\n", contador);
        if (contador == 5) { // Senvia cada 5 incrementos y se reinicia
            xQueueSend(cola, &contador, portMAX_DELAY);
            contador = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera de 1 segundo
    }
}

void inicia_contador(QueueHandle_t cola)
{
    xTaskCreatePinnedToCore(
        contador,              // Funcion de la tarea
        "contador",           // Nombre de task
        2048,                 // Tamaño de stack
        (void *) cola,       //  Parámetro de tarea
        4,                    // Prioridad de la tarea
        NULL,                 // Puntero a la tarea creada
        1);                   // Core 1
}