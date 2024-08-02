#ifndef STATE_H
#define STATE_H

enum State {
    MAIN_MENU,
    OPTION_1,
    OPTION_2,
    OPTION_3,
    WIFI
};

extern State currentState;

#endif // STATE_H
