#include "server.h"

void send_ws_message_worker(void *arg)
{
    uint8_t current_state = (uint8_t)((uint32_t)arg);

    char json_string[50];
    snprintf(json_string, sizeof(json_string), "{\"switches_state\":%d}", current_state);

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t *)json_string;
    ws_pkt.len = strlen(json_string);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    size_t max_clients = 4;
    size_t fds = max_clients;
    int client_fds[max_clients];

    if (httpd_get_client_list(server, &fds, client_fds) == ESP_OK)
    {
        for (size_t i = 0; i < fds; i++)
        {
            int sock = client_fds[i];

            if (httpd_ws_get_fd_info(server, sock) == HTTPD_WS_CLIENT_WEBSOCKET)
                httpd_ws_send_frame_async(server, sock, &ws_pkt);
        }
    }
}