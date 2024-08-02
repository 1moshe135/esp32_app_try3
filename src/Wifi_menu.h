#ifndef WIFI_MENU_H
#define WIFI_MENU_H

#include "menu_handling.h"

void scanWifiNetworks();
void deauthDevices();
void sniffPackets();
void wifiJamming();
void returnToMainMenu();

extern Menu wifiMenu;

#endif // WIFI_MENU_H
