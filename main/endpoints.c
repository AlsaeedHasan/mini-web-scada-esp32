#include "server.h"
#include "freertos/semphr.h"
#include "tasks.h"
#include "cJSON.h"

esp_err_t status_get_handler(httpd_req_t *req)
{

    uint8_t current_state = 0;

    if (xSemaphoreTake(state_mutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        current_state = switches_state;
        xSemaphoreGive(state_mutex);
    }
    else
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "switches_state", current_state);

    char *json_string = cJSON_PrintUnformatted(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, json_string);

    cJSON_Delete(root);
    free(json_string);

    return ESP_OK;
}

esp_err_t control_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    if (remaining >= sizeof(buf))
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    ret = httpd_req_recv(req, buf, remaining);
    if (ret <= 0)
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            httpd_resp_send_408(req);
        return ESP_FAIL;
    }

    buf[ret] = '\0';

    cJSON *root = cJSON_Parse(buf);
    if (root == NULL)
    {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON.");
        return ESP_FAIL;
    }

    cJSON *outputs_state = cJSON_GetObjectItem(root, "leds_state");

    if (outputs_state != NULL && cJSON_IsNumber(outputs_state))
    {
        uint8_t new_outputs_state = (uint8_t)outputs_state->valueint;

        if (xQueueSend(data_queue, &new_outputs_state, 0) == pdPASS)
        {
            httpd_resp_set_type(req, "application/json");
            httpd_resp_sendstr(req, "{\"status\":\"success\"}");
        }
        else
        {
            httpd_resp_send_500(req);
        }
    }
    else
    {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing or Invaling 'leds_state'");
    }

    cJSON_Delete(root);

    return ESP_OK;
}

httpd_uri_t status_uri = {
    .uri = "/api/status",
    .method = HTTP_GET,
    .handler = status_get_handler,
    .user_ctx = NULL,
};

httpd_uri_t control_uri = {
    .uri = "/api/control",
    .method = HTTP_POST,
    .handler = control_post_handler,
    .user_ctx = NULL,
};