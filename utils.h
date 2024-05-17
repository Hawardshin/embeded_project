#ifndef UTILS_H
#define UTILS_H

void disableRawMode();
void enableRawMode();
int initSerial();
void sendLCDMessage(int color);
void updateScoreDisplay();
void sendLEDMessage(int index, int state);
void updateLEDs();

extern int serial_fd;
extern int score;

#endif