#ifndef INPUT_H
#define INPUT_H

#define BUTTON_LEFT 114 
#define BUTTON_RIGHT 158 
#define BUTTON_UP 172 
#define BUTTON_DOWN 139

extern int lastButtonState_LEFT;
extern int lastButtonState_RIGHT;
extern int lastButtonState_UP;
extern int lastButtonState_DOWN;

void handleButtonEvent(int code, int value);

#endif