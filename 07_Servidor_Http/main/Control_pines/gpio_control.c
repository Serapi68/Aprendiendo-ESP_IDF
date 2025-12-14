#include "gpio_control.h"
#include "configuracion.h"
#include "driver/gpio.h"
#include "esp_log.h"


static bool led_state = false;

esp_err_t gpio_led_init(void) {
    esp_err_t ret = gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    if(ret != ESP_OK) {
        ESP_LOGE("GPIO", "Error al inicializar el GPIO del LED");
        return ret;
    }
    ret = gpio_set_level(LED_GPIO, LED_OFF);
    if(ret != ESP_OK) {
        ESP_LOGE("GPIO", "Error al establecer el nivel del LED");
        return ret;
    }
    led_state = false;
    ESP_LOGI("GPIO", "GPIO del LED inicializado correctamente");
    return ESP_OK;
}

esp_err_t gpio_led_set(led_state_t state)
{
    esp_err_t ret = gpio_set_level(LED_GPIO, state);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_GPIO, "Error al cambiar estado del LED");
        return ret;
    }

    led_state = (state == LED_ON);
    ESP_LOGI(TAG_GPIO, "LED %s", led_state ? "ENCENDIDO" : "APAGADO");
    return ESP_OK;
}

bool gpio_led_get_state(void)
{
    return led_state;
}