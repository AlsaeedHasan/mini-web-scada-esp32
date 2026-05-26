#include "server.h"
#include "freertos/semphr.h"
#include "tasks.h"
#include "cJSON.h"

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");

    const size_t resp_len = index_html_end - index_html_start;

    httpd_resp_send(req, (const char *)index_html_start, resp_len);

    return ESP_OK;
}

esp_err_t ws_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET)
        return ESP_OK;

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK)
        return ret;

    if (ws_pkt.len == 0)
        return ESP_OK;

    uint8_t *buf = calloc(1, ws_pkt.len + 1);
    if (buf == NULL)
        return ESP_ERR_NO_MEM;

    ws_pkt.payload = buf;

    ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
    if (ret == ESP_OK)
    {
        cJSON *root = cJSON_Parse((char *)ws_pkt.payload);
        if (root != NULL)
        {
            cJSON *outputs_state = cJSON_GetObjectItem(root, "leds_state");
            if (outputs_state != NULL && cJSON_IsNumber(outputs_state))
            {
                uint8_t new_outputs_state = (uint8_t)outputs_state->valueint;
                xQueueSend(data_queue, &new_outputs_state, 0);
            }
            cJSON_Delete(root);
        }
    }

    free(buf);

    return ESP_OK;
}

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

httpd_uri_t root_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler,
    .user_ctx = NULL,
};

httpd_uri_t ws_uri = {
    .uri = "/ws",
    .method = HTTP_GET,
    .handler = ws_handler,
    .user_ctx = NULL,
    .is_websocket = true,
};