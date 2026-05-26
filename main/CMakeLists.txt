idf_component_register(
    SRCS "endpoints.c" "server.c" "pd_wifi.c" "tasks.c" "shift_registers.c" "pins.c" "main.c"
    INCLUDE_DIRS "include"
    REQUIRES cjson network_provisioning nvs_flash esp_wifi esp_http_server)
