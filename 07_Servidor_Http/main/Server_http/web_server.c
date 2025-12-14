#include "web_server.h"
#include "auth.h"
#include "configuracion.h"
#include "gpio_control.h"
#include <string.h>
#include "esp_log.h"


// Archivos HTML embebidos
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

static esp_err_t root_get_handler(httpd_req_t *req)
{
    // Verificar autenticación
    if (check_auth(req) != ESP_OK) {
        return ESP_OK; // Autenticación fallida
    }
    const uint32_t index_html_len = index_html_end - index_html_start;

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_len);

    return ESP_OK;
}

// Manejador para encender y apagar el LED
static esp_err_t led_jueguru(httpd_req_t *req){
    if(check_auth(req) != ESP_OK) {
        return ESP_OK; 
    }
    // Obtener el valor del query string
    char query[32];
    if( httpd_req_get_url_query_str(req, query, sizeof(query)) != ESP_OK ) {
        httpd_resp_send_404(req);
        return ESP_OK;  
    }

    // Procesar el comando
    if(strstr(query, "on")) {
        gpio_led_set(LED_ON);
    } else if(strstr(query, "off")) {
        gpio_led_set(LED_OFF);
    } else {
        httpd_resp_send_404(req);
        return ESP_OK;  
    }
    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}

// Estructura para las rutas del servidor web

typedef struct {
    const char *uri;
    httpd_method_t method;
    esp_err_t (*handler)(httpd_req_t *req);
} web_route_t;

//Definicion de rutas 

static const web_route_t routes[] = {
    { "/", HTTP_GET, root_get_handler },
    { "/led", HTTP_GET, led_jueguru },
};

httpd_handle_t web_server_start(void){
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = HTTP_PORT;

    // Iniciar el servidor HTTP
    esp_err_t ret = httpd_start(&server, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_WEB, "Error al iniciar el servidor web: %s", esp_err_to_name(ret));
        return NULL;
    }

    ESP_LOGI(TAG_WEB, "Servidor web iniciado en el puerto %d", config.server_port);

    // Registrar las rutas
    for (int i = 0; i < sizeof(routes) / sizeof(routes[0]); i++){
        httpd_uri_t uri_handler = {
            .uri       = routes[i].uri,
            .method    = routes[i].method,
            .handler   = routes[i].handler,
            .user_ctx  = NULL
        };
        ret = httpd_register_uri_handler(server, &uri_handler);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG_WEB, "Error al registrar la ruta %s: %s", routes[i].uri, esp_err_to_name(ret));
        } else {
            ESP_LOGI(TAG_WEB, "Ruta registrada: %s", routes[i].uri);
        }
    }

    return server;

}

esp_err_t web_server_stop(httpd_handle_t server)
{
    if (server == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = httpd_stop(server);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_WEB, "Error al detener servidor HTTP: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG_WEB, "Servidor HTTP detenido");
    }
    
    return ret;
}
