#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include "game.h"
#include "graphics.h"
#include "sound.h"
#include "input.h"
#include "utils.h"

int main() {
    int fd = open("/dev/input/event1", O_RDONLY);
    if (fd == -1) {
        perror("Opening input device");
        return EXIT_FAILURE;
    }

    buzzer_fd = open("/dev/buzzer", O_RDONLY);
    if (buzzer_fd == -1) {
        perror("Opening buzzer device");
        close(fd);
        return EXIT_FAILURE;
    }

    if (initSerial() != 0) {
        close(fd);
        close(buzzer_fd);
        return EXIT_FAILURE;
    }

    enableRawMode();
    initGame();
    drawField();

    struct input_event ev;
    while (!game_over) {
        if (read(fd, &ev, sizeof(struct input_event)) > 0 && ev.type == 1) {
            handleButtonEvent(ev.code, ev.value);
        }
        playBackgroundMusic(); // 배경음악 재생
        updatePacman();
        updateGhosts();
        updateLEDs(); // LED 업데이트
        drawField();
        struct timespec ts = {0, 200000000}; // 0.2초
        nanosleep(&ts, NULL);
    }

    printf("Game Over! Your score is %d\n", score);
    playGameOverSound();
    sendLCDMessage(0);
    disableRawMode();
  
    close(fd);
    close(buzzer_fd);
    close(serial_fd);
    return 0;
}