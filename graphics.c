#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void drawField() {
    system("clear");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (isWall(x, y)) {
                printf("\033[0;34m#\033[0m"); // 벽을 파란색으로 표시
            } else if (pacman.position.x == x && pacman.position.y == y) {
                printf("\033[0;33mP\033[0m"); // Pacman을 노란색으로 표시
            } else {
                int is_ghost = 0;
                for (int i = 0; i < 4; i++) {
                    if (ghosts[i].position.x == x && ghosts[i].position.y == y) {
                        is_ghost = 1;
                        break;
                    }
                }
                if (is_ghost) {
                    printf("\033[0;31mG\033[0m"); // 고스트를 빨간색으로 표시
                } else if (isPellet(x, y)) {
                    printf("\033[0;32m.\033[0m"); // 펠릿을 녹색으로 표시
                } else if (isPowerup(x, y)) {
                    printf("\033[0;35m*\033[0m"); // 파워업을 보라색으로 표시
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("Level: %d\n", level);
    if (pacman.powerup_active) {
        printf("Power-Up Active!\n");
    }
}