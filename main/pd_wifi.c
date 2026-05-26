#include "pd_wifi.h"
#include "server.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "network_provisioning/manager.h"
#include "network_provisioning/scheme_softap.h"
#include "freertos/event_groups.h"

EventGroupHandle_t wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);

void wifi_init_basic(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void start_network_provisioning(void)
{
    network_prov_mgr_config_t config = {
        .scheme = network_prov_scheme_softap,
        .scheme_event_handler = NETWORK_PROV_EVENT_HANDLER_NONE};
    ESP_ERROR_CHECK(network_prov_mgr_init(config));

    bool isprovisioned = true;
    ESP_ERROR_CHECK(network_prov_mgr_is_wifi_provisioned(&isprovisioned));

    if (!isprovisioned)
    {
        printf("Starting Provisioning Mode...\n");
        const char *service_name = "SA3ED ESP";
        const char *pop = "SA3ED-ESP-2026-SCADA";

        network_prov_mgr_start_provisioning(NETWORK_PROV_SECURITY_1, pop, service_name, NULL);
        printf("SoftAP Provisioning started! Connect to %s and enter PoP: %s\n", service_name, pop);
    }
    else
    {
        printf("Already provisioned, starting Wi-Fi directly...\n");

        network_prov_mgr_deinit();
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
    }
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
        esp_wifi_connect();

    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        esp_wifi_connect();
    }

    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        printf("Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        printf("Wi-Fi Connected and Ready!\n");

        if (server == NULL)
        {
            start_webserver();
        }
    }
}