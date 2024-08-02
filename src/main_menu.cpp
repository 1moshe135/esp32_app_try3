#include "main_menu.h"
#include "wifi_menu.h"
#include "state.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>

extern Adafruit_ST7789 tft;

void option1() {
    Serial.println("Test 1 selected");
    currentState = OPTION_1;
    tft.fillScreen(ST77XX_BLUE); // Ensure background is white
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(3);
    tft.print("Option 1");
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.print("Press.. if you dare..");
}

void option2() {
    Serial.println("Test 2 selected");
    currentState = OPTION_2;
    tft.fillScreen(ST77XX_RED); // Ensure background is white
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(3);
    tft.print("Option 2");
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.print("Press >:3");
}

void option3() {
    Serial.println("Test 3 selected");
    currentState = OPTION_3;
    tft.fillScreen(ST77XX_CYAN); // Ensure background is white
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(3);
    tft.print("Option 3");
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.print("Press :)");
}

void openWifiMenu() {
    currentState = WIFI;
    tft.fillScreen(ST77XX_WHITE); // Ensure background is white
    wifiMenu.drawMenu(tft);
}

// Menu items for the main menu
MenuItem mainMenuItems[] = {
    {"Wifi", openWifiMenu},
    {"test 1", option1},
    {"test 2", option2},
    {"test 3", option3}
};

// Create Main Menu object
Menu mainMenu("Main Menu", mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0]));
