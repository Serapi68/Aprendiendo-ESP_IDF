# _FreeRTOS HOLA MUNDO_

Primer paso en FreeRTOS, creación de tareas básicas y manejo de scheduling en ESP-IDF

## SECUENCIA DE CÓDIGO

El proyecto contiene un archivo principal donde se definen las tareas de FreeRTOS:
    
    main.c

En este archivo se configuran las tareas que ejecutarán diferentes funciones en paralelo:
    
    xTaskCreate(tarea_led, "LED_Task", 2048, NULL, 1, NULL);
    xTaskCreate(tarea_consola, "Console_Task", 2048, NULL, 1, NULL);

Se utilizan las funciones de FreeRTOS como:
    
    vTaskDelay() - Para pausar la ejecución de una tarea
    xTaskCreate() - Para crear nuevas tareas
    printf() - Para imprimir en consola

## RITMO MUSICAL: TRES CONTRA DOS

El código implementa un patrón de ritmo musical conocido como "tres contra dos" (polyrhythm 3:2):
    
    tarea_led - Ejecuta un patrón cada 2 unidades de tiempo
    tarea_consola - Ejecuta un patrón cada 3 unidades de tiempo

Cuando ambas tareas se ejecutan concurrentemente, crean una secuencia sincronizada donde los patrones se alinean cada 6 ciclos, generando el efecto rítmico característico usado en géneros como el jazz, funk y música africana.


## NOTAS IMPORTANTES

- Las tareas en FreeRTOS son funciones infinitas que se ejecutan concurrentemente
- Usa vTaskDelay() para ceder control a otras tareas
- El scheduler de FreeRTOS cambia entre tareas automáticamente
- FreeRTOS facilita la recreación de patrones polimétricos al permitir que múltiples tareas se ejecuten independientemente con sus propios intervalos de tiempo, sincronizándose automáticamente a través del scheduler

## VIDEO
Se evidencia el patron ritmico simulado:




https://github.com/user-attachments/assets/b0f5d0f7-357b-4c58-a1eb-537ec0df443f

