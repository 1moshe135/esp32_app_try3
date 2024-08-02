#include "menu_handling.h"
#include <Arduino.h>

Menu::Menu(const char* name, MenuItem* items, int length, Menu* prevMenu)
    : menuName(name), menuItems(items), menuLength(length), currentOption(0), previousMenu(prevMenu) {}

void Menu::initDisplay(Adafruit_ST7789 &tft) {
    tft.init(240, 320); // Init ST7789 240x320
    tft.setRotation(1); // Set display rotation
    tft.fillScreen(ST77XX_WHITE);
}

void Menu::drawMenu(Adafruit_ST7789 &tft) {
    tft.fillScreen(ST77XX_WHITE);
    tft.setTextSize(3);
    tft.setCursor(10, 10);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(menuName);

    tft.setTextSize(2);
    for (int i = 0; i < menuLength; i++) {
        tft.setCursor(10, 60 + i * 30);
        if (i == currentOption) {
            tft.setTextColor(ST77XX_BLACK);
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "> %s", menuItems[i].name);
            tft.print(buffer);
        } else {
            tft.setTextColor(PURPLE);
            tft.print(menuItems[i].name);
        }
    }

    if (previousMenu) {
        tft.setCursor(10, 60 + menuLength * 30);
        if (currentOption == menuLength) {
            tft.setTextColor(ST77XX_BLACK);
            tft.print("> Back");
        } else {
            tft.setTextColor(PURPLE);
            tft.print("Back");
        }
    }
}

void Menu::handleJoystick(Adafruit_ST7789 &tft) {
    static int lastX = 0, lastY = 0;

    int x = analogRead(34);
    int y = analogRead(35);
    bool selectPressed = digitalRead(32) == LOW;

    //Serial.print("X: "); Serial.print(x); Serial.print(" Y: "); Serial.print(y); Serial.print(" Select: "); Serial.println(selectPressed);

    if (abs(x - lastX) > 100 || abs(y - lastY) > 100) {
        lastX = x;
        lastY = y;
        if (x > 3000) {
            // Move up
            currentOption = (currentOption - 1 + (menuLength + (previousMenu ? 1 : 0))) % (menuLength + (previousMenu ? 1 : 0));
            drawMenu(tft);
        } else if (x < 1000) {
            // Move down
            currentOption = (currentOption + 1) % (menuLength + (previousMenu ? 1 : 0));
            drawMenu(tft);
        }
    }

    if (selectPressed) {
        Serial.println("Select pressed");
        if (previousMenu && currentOption == menuLength) {
            // Go back to the previous menu
            Serial.println("Going back to previous menu");
            previousMenu->initDisplay(tft);
            previousMenu->drawMenu(tft);
        } else {
            // Call the callback function
            Serial.print("Calling callback for: "); Serial.println(menuItems[currentOption].name);
            menuItems[currentOption].callback();
        }

        delay(100); // Debounce delay
    }

    delay(100); // Debounce delay
}
