#ifndef MENU_HANDLING_H
#define MENU_HANDLING_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Custom color definition
#define PURPLE 0x780F

typedef void (*MenuCallback)();

struct MenuItem {
    const char* name;
    MenuCallback callback;
};

class Menu {
public:
    Menu(const char* name, MenuItem* items, int length, Menu* prevMenu = nullptr);
    void initDisplay(Adafruit_ST7789 &tft);
    void drawMenu(Adafruit_ST7789 &tft);
    void handleJoystick(Adafruit_ST7789 &tft);

private:
    const char* menuName;
    MenuItem* menuItems;
    int menuLength;
    int currentOption;
    Menu* previousMenu;
};

#endif // MENU_HANDLING_H
