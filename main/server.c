#include "server.h"
#include "esp_http_server.h"
#include "tasks.h"

httpd_handle_t server = NULL;

httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        printf("Server started successfully on port: %d\n", config.server_port);

        httpd_register_uri_handler(server, &status_uri);
        httpd_register_uri_handler(server, &control_uri);

        return server;
    }

    printf("Error starting server!\n");
    return NULL;
}
