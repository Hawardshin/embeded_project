#include "input.h"
#include "game.h"

int lastButtonState_LEFT = 0;
int lastButtonState_RIGHT = 0;
int lastButtonState_UP = 0;
int lastButtonState_DOWN = 0;

void handleButtonEvent(int code, int value) {
    switch (code) {
        case BUTTON_LEFT:
            if (value == 1 && lastButtonState_LEFT == 0) {
                pacman.direction = LEFT;
                lastButtonState_LEFT = 1;
            } else if (value == 0) {
                lastButtonState_LEFT = 0;
            }
            break;
        case BUTTON_RIGHT:
            if (value == 1 && lastButtonState_RIGHT == 0) {
                pacman.direction = RIGHT;
                lastButtonState_RIGHT = 1;
            } else if (value == 0) {
                lastButtonState_RIGHT = 0;
            }
            break;
        case BUTTON_UP:
            if (value == 1 && lastButtonState_UP == 0) {
                pacman.direction = UP;
                lastButtonState_UP = 1;
            } else if (value == 0) {
                lastButtonState_UP = 0;
            }
            break;
        case BUTTON_DOWN:
            if (value == 1 && lastButtonState_DOWN == 0) {
                pacman.direction = DOWN;
                lastButtonState_DOWN = 1;
            } else if (value == 0) {
                lastButtonState_DOWN = 0;
            }
            break;
    }
}
