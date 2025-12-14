#include "auth.h"
#include "configuracion.h"
#include <string.h>
#include "esp_log.h"
#include "mbedtls/base64.h"



esp_err_t send_unauthorized_response(httpd_req_t *req) {

    httpd_resp_set_status(req, "401 Unauthorized");
    httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"" AUTH_REALM "\"");
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_sendstr(req, "Acceso denegado");
    return ESP_FAIL;
}

esp_err_t check_auth(httpd_req_t *req) {
    // Obtener longitud del header Authorization
    size_t auth_len = httpd_req_get_hdr_value_len(req, "Authorization") + 1;
    if (auth_len <= 1) {
        return send_unauthorized_response(req);
    }

    // Obtener el valor del header Authorization
    char auth_header[128];
    if (httpd_req_get_hdr_value_str(req, "Authorization", auth_header, sizeof(auth_header)) != ESP_OK) {
        return send_unauthorized_response(req);
    }

    // Verificar que el esquema sea Basic
    if (strncmp(auth_header, "Basic ", 6) != 0) {
        return send_unauthorized_response(req);
    }

    // Decodificar base64 con mbedtls
    size_t decoded_len = 0;
    unsigned char decoded[128];

    int ret = mbedtls_base64_decode(decoded, 
                                     sizeof(decoded), 
                                     &decoded_len,
                                     (const unsigned char*)(auth_header + 6),
                                     strlen(auth_header + 6)
                                    );

    if (ret != 0) {
        return send_unauthorized_response(req);
    }

    decoded[decoded_len] = '\0';
    // Separar usuario y contraseña

    char *colon = strchr((char*)decoded, ':');
    if (!colon) {
        return send_unauthorized_response(req);
    }

    *colon = '\0';
    char *user = (char*)decoded;
    char *pass = colon + 1;

    if (strcmp(user, AUTH_USERNAME) == 0 && strcmp(pass, AUTH_PASSWORD) == 0) {
        ESP_LOGD(TAG_AUTH, "Autenticación exitosa para el usuario:");
        return ESP_OK;
    }

    ESP_LOGW(TAG_AUTH, "Fallo de autenticación para el usuario: ");
    

    return send_unauthorized_response(req);
}

