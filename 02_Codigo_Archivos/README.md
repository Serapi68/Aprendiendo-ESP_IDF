# _SEGUNDO CÓDIGO_

Segundo paso, división de varias funciones por diversos archivos

## SECUENCIA DE CÓDIGO

Se han usado dos archivos para definir secuencias:
    
    un_led.c
    varios_led.c

Se hace una cabecera de las mismas funciones
    
    un_led.h
    varios_led.h

Se llaman las cabeceras en _main.c_:
    
    #include "un_led.h"
    #include "varios_led.h"

Por último se ponen en la secuencia de main:
        
        if (boton_estado == 0)
        {
            configurar_leds();
        }
        else
        {
            manejo_led();
        }

## CÓMO FUNCIONA EL BUILD_ESP_IDF

En el archivo _CMakeLists.txt_ de la carpeta _main_ se tendrán que declarar los archivos que queremos incluir en el _build_ de esp_idf, si no se hace esto cuando queramos flashear en nuestra ESP_32 no se podrá, ejemplo:
    
    idf_component_register(SRCS "main.c" "varios_led.c" "un_led.c"
                    INCLUDE_DIRS ".")

## VIDEO:


https://github.com/user-attachments/assets/a3a138ec-0456-4ed1-9391-34157a6d73a0

