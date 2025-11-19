#include "luces.h"
#include "configuracion_wifi.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define LED_PIN GPIO_NUM_2 // Pin del LED integrado en la mayoría de las placas ESP32
#define LED_PIN2 GPIO_NUM_4 // Pin del LED integrado en la mayoría de las placas ESP32

extern int conexiones;  

static void inicia_leds(void *pvParameter)
{
    gpio_set_direction(LED_PIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    gpio_set_level(LED_PIN2, 0);
    gpio_set_level(LED_PIN, 0);

    vTaskDelete(NULL);

}

void actualizar_led(int conexiones)
{
    gpio_set_level(LED_PIN, conexiones >= 1 ? 1 : 0);
    gpio_set_level(LED_PIN2, conexiones >= 2 ? 1 : 0);
}

void luces_init(void)
{
    xTaskCreatePinnedToCore(inicia_leds, "inicia_leds", 8192, NULL, 5, NULL, 0);
}