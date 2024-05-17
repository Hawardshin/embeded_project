#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include "utils.h"

struct termios orig_termios;
int serial_fd = -1;
extern int score;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int initSerial() {
    struct termios tio;

    serial_fd = open("/dev/ttymxc1", O_RDWR);
    if (serial_fd == -1) {
        perror("Opening serial device");
        return 1;
    }

    bzero(&tio, sizeof(tio));
    tio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL;
    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;
    tio.c_lflag = 0;
    tio.c_cc[VTIME] = 0;
    tio.c_cc[VMIN] = 5;

    if (tcsetattr(serial_fd, TCSANOW, &tio) != 0) {
        perror("Setting serial attributes");
        return 1;
    }

    return 0;
}

void sendLCDMessage(int color) {
    if (serial_fd != -1) {
        unsigned char msg[] = { 0x12, 0x24, 0x30, 0x30 + color, 0x13 };
        write(serial_fd, msg, sizeof(msg));
    }
}

void updateScoreDisplay() {
    char hundreds = (score / 100) % 10;
    char tens = (score / 10) % 10;
    char units = score % 10;

    unsigned char msg_hundreds[] = { 0x12, 0x23, 0x30 + 0, 0x30 + hundreds, 0x13 };
    unsigned char msg_tens[] = { 0x12, 0x23, 0x30 + 1, 0x30 + tens, 0x13 };
    unsigned char msg_units[] = { 0x12, 0x23, 0x30 + 2, 0x30 + units, 0x13 };

    write(serial_fd, msg_hundreds, sizeof(msg_hundreds));
    write(serial_fd, msg_tens, sizeof(msg_tens));
    write(serial_fd, msg_units, sizeof(msg_units));
}

void sendLEDMessage(int index, int state) {
    if (serial_fd != -1) {
        unsigned char msg[] = { 0x12, 0x21, 0x30 + index, 0x30 + state, 0x13 };
        write(serial_fd, msg, sizeof(msg));
    }
}

void updateLEDs() {
    static int currentLED = 0;
    sendLEDMessage(currentLED, 1);
    usleep(200000); // LED 켜짐 시간
    sendLEDMessage(currentLED, 0);
    currentLED = (currentLED + 1) % 5;
}
