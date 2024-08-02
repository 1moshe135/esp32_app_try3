#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "menu_handling.h"
#include "main_menu.h"
#include "wifi_menu.h"
#include "state.h"

// TFT display pins
#define TFT_CS     15
#define TFT_RST    4
#define TFT_DC     2
#define TFT_MOSI   23
#define TFT_SCK    18

// Joystick pins
#define JOY_SEL    32 // Button select
#define JOY_X      34 // X-axis
#define JOY_Y      35 // Y-axis
#define JOY_VCC    5  // VCC for joystick

// Create display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void returnToMenu() {
    currentState = MAIN_MENU;
    tft.fillScreen(ST77XX_WHITE);
    mainMenu.drawMenu(tft);
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);

    // Initialize TFT display
    mainMenu.initDisplay(tft);

    // Initialize joystick pins
    pinMode(JOY_SEL, INPUT_PULLUP);
    pinMode(JOY_X, INPUT);
    pinMode(JOY_Y, INPUT);

    // Supply VCC to joystick
    pinMode(JOY_VCC, OUTPUT);
    digitalWrite(JOY_VCC, HIGH);

    // Draw initial menu
    mainMenu.drawMenu(tft);
}

void loop() {
    if (currentState == MAIN_MENU) {
        mainMenu.handleJoystick(tft);
    } else if (currentState == WIFI) {
        wifiMenu.handleJoystick(tft);
    } else {
        // Check if the joystick select button is pressed to return to the menu
        if (digitalRead(JOY_SEL) == LOW) {
            delay(200); // Debounce delay
            returnToMenu();
        }
    }
}
