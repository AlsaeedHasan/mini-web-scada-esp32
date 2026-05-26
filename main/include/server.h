#ifndef SERVER_H_
#define SERVER_H_

#include "esp_http_server.h"

extern httpd_handle_t server;
extern httpd_uri_t status_uri;
extern httpd_uri_t control_uri;
extern httpd_uri_t root_uri;
extern httpd_uri_t ws_uri;

httpd_handle_t start_webserver(void);

esp_err_t status_get_handler(httpd_req_t *req);
esp_err_t control_post_handler(httpd_req_t *req);
esp_err_t root_get_handler(httpd_req_t *req);

#endif