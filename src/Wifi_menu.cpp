#include "wifi_menu.h"
#include "main_menu.h"
#include "state.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <Arduino.h>
#include <esp_wifi.h>

extern Adafruit_ST7789 tft;

int packetCount = 0;
const int maxPacketsToDisplay = 10;
String packetDetails[maxPacketsToDisplay];

// Define the IEEE 802.11 MAC header struct
typedef struct {
    uint8_t frame_ctrl[2];
    uint8_t duration_id[2];
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint8_t seq_ctrl[2];
    uint8_t addr4[6];  // Optional, present only in data frames with ToDS and FromDS set
} wifi_ieee80211_mac_hdr_t;

void wifiSnifferPacketHandler(void *buff, wifi_promiscuous_pkt_type_t type) {
    if (type != WIFI_PKT_MGMT && type != WIFI_PKT_DATA) {
        return;
    }
    packetCount++;
    
    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buff;
    wifi_ieee80211_mac_hdr_t *hdr = (wifi_ieee80211_mac_hdr_t *)pkt->payload;

    char packetInfo[128];
    snprintf(packetInfo, sizeof(packetInfo), "Packet %d: %02x:%02x:%02x:%02x:%02x:%02x -> %02x:%02x:%02x:%02x:%02x:%02x",
             packetCount,
             hdr->addr2[0], hdr->addr2[1], hdr->addr2[2], hdr->addr2[3], hdr->addr2[4], hdr->addr2[5],
             hdr->addr1[0], hdr->addr1[1], hdr->addr1[2], hdr->addr1[3], hdr->addr1[4], hdr->addr1[5]);

    if (packetCount <= maxPacketsToDisplay) {
        packetDetails[packetCount - 1] = String(packetInfo);
    }
}

void sniffPackets() {
    Serial.println("Sniffing packets...");
    packetCount = 0;

    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.print("Sniffing packets...");

    // Clear packet details array
    for (int i = 0; i < maxPacketsToDisplay; i++) {
        packetDetails[i] = "";
    }

    // Set WiFi to promiscuous mode
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_NULL);
    esp_wifi_start();
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&wifiSnifferPacketHandler);

    // Sniff packets for a certain amount of time
    unsigned long startMillis = millis();
    while (millis() - startMillis < 20000) {  // Sniff for 20 seconds
        delay(100);
    }

    esp_wifi_set_promiscuous(false);

    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.print("Packet sniffing done");
    tft.setCursor(10, 30);
    tft.print("Packets captured: ");
    tft.print(packetCount);
    tft.setCursor(10, 50);

    // Display captured packet details
    for (int i = 0; i < maxPacketsToDisplay && i < packetCount; i++) {
        tft.println(packetDetails[i]);
        if ((i + 1) * 20 + 50 > 320) {
            break; // Prevent text from overflowing the screen
        }
        tft.setCursor(10, 50 + (i + 1) * 20);
    }
    tft.setCursor(10, 300);
    tft.print("Press to go back");
}

void scanWifiNetworks() {
    Serial.println("Scanning WiFi networks...");
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.print("Scanning WiFi networks...");

    int numNetworks = WiFi.scanNetworks();
    delay(1000); // Allow some time for the user to see the scanning message
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.setTextSize(2);

    if (numNetworks == 0) {
        tft.print("No networks found.");
    } else {
        for (int i = 0; i < numNetworks; ++i) {
            tft.print(i + 1);
            tft.print(": ");
            tft.print(WiFi.SSID(i));
            tft.print(" (");
            tft.print(WiFi.RSSI(i));
            tft.println(" dBm)");
            tft.setCursor(10, 30 + i * 20);
            if (i >= 10) break; // Limit to displaying 10 networks
        }
    }
    tft.setCursor(10, 300);
    tft.print("Press to go back");
}

void deauthDevices() {
    Serial.println("Deauthenticating devices...");
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.print("Deauthenticating devices...");
    // Add your deauthentication code here
    delay(2000);
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.print("Deauthentication done");
    tft.setCursor(10, 300);
    tft.print("Press to go back");
}

void wifiJamming() {
    Serial.println("Jamming WiFi...");
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.print("Jamming WiFi...");
    // Add your WiFi jamming code here
    delay(2000);
    tft.fillScreen(ST77XX_WHITE);
    tft.setCursor(10, 10);
    tft.print("WiFi jamming done");
    tft.setCursor(10, 300);
    tft.print("Press to go back");
}

void returnToMainMenu() {
    currentState = MAIN_MENU;
    tft.fillScreen(ST77XX_WHITE);
    mainMenu.drawMenu(tft);
}

// WiFi menu items
MenuItem wifiMenuItems[] = {
    {"Scan WiFi", scanWifiNetworks},
    {"Deauth Devices", deauthDevices},
    {"Sniff Packets", sniffPackets},
    {"WiFi Jamming", wifiJamming},
    {"Back", returnToMainMenu}
};

// Create WiFi Menu object
Menu wifiMenu("WiFi Menu", wifiMenuItems, sizeof(wifiMenuItems) / sizeof(wifiMenuItems[0]));
