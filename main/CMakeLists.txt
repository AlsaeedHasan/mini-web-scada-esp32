idf_component_register(
    SRCS "pd_wifi.c" "tasks.c" "shift_registers.c" "pins.c" "main.c"
    INCLUDE_DIRS "include"
    REQUIRES network_provisioning nvs_flash esp_wifi)
