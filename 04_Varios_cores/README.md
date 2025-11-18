# Varios Cores

Segundo paso en FreeRTOS: dividir el proyecto en varios archivos y crear tareas ejecutándose en núcleos diferentes.

## SECUENCIA DE CÓDIGO

El proyecto contiene un archivo principal donde se crean las tareas de FreeRTOS:

    main.c

Tenemos varios archivos auxiliares como:

    boton.c
    contador.c
    parpadeo.c 

En estos se implementa la lógica requerida, se verifica el envío de información entre núcleos y se crea un sistema para visualizar las tareas en ejecución mediante un buffer en el monitor de ESP-IDF, utilizando una cola Queue y funciones de FreeRTOS:

    contador_cola = xQueueCreate(10, sizeof(int)); // Se ubica en el archivo main.c
    xQueueSend(cola, &contador, portMAX_DELAY);     // Se enviará la información del contador
    xQueueReceive(cola, &valor_recibido, pdMS_TO_TICKS(10)) // Lo enviado se almacenará en valor_recibido, logrando la conexión

Se crean las tareas que se ejecutaran, cambiando su prioridad y su nucleo de trabajo, se muestra un ejemplo:

    xTaskCreatePinnedToCore(
        parpadeo_led,          // Funcion de la tarea
        "parpadeo_led",       // Nombre de task
        2048,                 // Tamaño de stack
        (void *) cola,       //  Parámetro de tarea
        5,                    // Prioridad de la tarea
        NULL,                 // Puntero a la tarea creada
        0);                   // Core 0


Se utilizan las funciones de FreeRTOS como:
    xQueueCreate() - Para crear colas
    xQueueSend() / xQueueReceive() – Comunicación entre tareas
    vTaskList() - Para extraer la lista de tareas en ejecusion
    vTaskDelay() - Para pausar la ejecución de una tarea
    xTaskCreate() - Para crear nuevas tareas
    printf() - Para imprimir en consola


# RESULTADO
BOTÓN PULSADO → Mostrando Tareas:
Nombre      Estado   Prioridad  Stack   Num   Core
------------------------------------------------------
parpadeo_led  R       5          380     5     0
contador      B       4          420     6     1
boton         B       3          460     7     0
IDLE0         R       0          110     1     0
IDLE1         B       0          110     2     1


## NOTAS IMPORTANTES

-Es necesario abrir la configuración en ESP-IDF: SDK Configuration editor (menuconfig), ir a Component config → FreeRTOS → Kernel y habilitar Enable FreeRTOS stats formatting functions para que vTaskList() funcione.

-Para usar colas Queue es obligatorio incluir:
    #include "freertos/queue.h"


## VIDEO
Se evidencia la secuencia hecha:


