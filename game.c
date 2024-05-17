#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "sound.h"
#include "graphics.h"
#include "utils.h"

Pacman pacman;
Pellet pellets[WIDTH * HEIGHT];
Pellet powerups[WIDTH * HEIGHT];
Ghost ghosts[4];
int num_pellets = 0;
int num_powerups = 0;
int game_over = 0;
int score = 0;
int level = 1;
int pellets_to_eat = 5; // 처음에 먹어야 하는 펠릿 수

void initGame() {
    pacman.position = (Point){WIDTH / 2, HEIGHT / 2};
    pacman.direction = RIGHT;
    pacman.powerup_active = 0;
    pacman.powerup_duration = 0;
    game_over = 0;
    score = 0;
    level = 1;
    pellets_to_eat = 5; // 초기 값

    // Initialize pellets
    num_pellets = 0;
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            if (rand() % 4 == 0) { // 초기에는 맵에 25%의 확률로 펠릿 배치
                pellets[num_pellets++] = (Pellet){x, y};
            }
        }
    }

    // Initialize powerups
    num_powerups = 0;
    for (int i = 0; i < 5; i++) {
        powerups[num_powerups++] = (Pellet){rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
    }

    // Initialize ghosts
    srand(time(NULL));
    for (int i = 0; i < 4; i++) {
        ghosts[i].position = (Point){rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
        ghosts[i].direction = rand() % 4;
    }

    playGameStartSound();
    sendLCDMessage(3); // 로고 화면
    updateScoreDisplay(); // 게임 시작 시 점수를 000으로 맞춤
}

void initLevel() {
    pacman.position = (Point){WIDTH / 2, HEIGHT / 2};
    pacman.direction = RIGHT;
    pacman.powerup_active = 0;
    pacman.powerup_duration = 0;
    game_over = 0;

    // Initialize pellets
    num_pellets = 0;
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            if (rand() % (6 - level) == 0) { // 레벨이 올라갈수록 펠릿 배치 확률 증가
                pellets[num_pellets++] = (Pellet){x, y};
            }
        }
    }

    // Initialize powerups
    num_powerups = 0;
    for (int i = 0; i < 5; i++) {
        powerups[num_powerups++] = (Pellet){rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
    }

    // Initialize ghosts
    srand(time(NULL));
    for (int i = 0; i < 4; i++) {
        ghosts[i].position = (Point){rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
        ghosts[i].direction = rand() % 4;
    }
}

int isWall(int x, int y) {
    return (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1);
}

int isPellet(int x, int y) {
    for (int i = 0; i < num_pellets; i++) {
        if (pellets[i].x == x && pellets[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int isPowerup(int x, int y) {
    for (int i = 0; i < num_powerups; i++) {
        if (powerups[i].x == x && powerups[i].y == y) {
            return 1;
        }
    }
    return 0;
}

void removePellet(int x, int y) {
    for (int i = 0; i < num_pellets; i++) {
        if (pellets[i].x == x && pellets[i].y == y) {
            pellets[i] = pellets[--num_pellets];
            return;
        }
    }
}

void removePowerup(int x, int y) {
    for (int i = 0; i < num_powerups; i++) {
        if (powerups[i].x == x && powerups[i].y == y) {
            powerups[i] = powerups[--num_powerups];
            return;
        }
    }
}

void updatePacman() {
    Point new_position = pacman.position;
    switch (pacman.direction) {
        case UP:    new_position.y--; break;
        case DOWN:  new_position.y++; break;
        case LEFT:  new_position.x--; break;
        case RIGHT: new_position.x++; break;
    }

    if (!isWall(new_position.x, new_position.y)) {
        pacman.position = new_position;
    }

    if (isPellet(pacman.position.x, pacman.position.y)) {
        removePellet(pacman.position.x, pacman.position.y);
        score += PELLET_SCORE;
        pellets_to_eat--;
        playPelletSound();
        sendLCDMessage(1); // 펠릿 수집 시 녹색
        updateScoreDisplay();
        if (score >= MAX_SCORE) {
            printf("Victory! You reached the maximum score of %d!\n", MAX_SCORE);
            playVictorySound();
            sendLCDMessage(2); // 승리 시 파란색
            game_over = 1;
            return;
        }
    }

    if (isPowerup(pacman.position.x, pacman.position.y)) {
        removePowerup(pacman.position.x, pacman.position.y);
        score += POWERUP_SCORE;
        pacman.powerup_active = 1;
        pacman.powerup_duration = 10; // 10턴 동안 파워업 지속
        playPowerupSound();
        sendLCDMessage(2); // 파워업 시 파란색
        updateScoreDisplay();
        if (score >= MAX_SCORE) {
            printf("Victory! You reached the maximum score of %d!\n", MAX_SCORE);
            playVictorySound();
            sendLCDMessage(3); // 승리 시 초기화면
            game_over = 1;
            return;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (pacman.position.x == ghosts[i].position.x &&
            pacman.position.y == ghosts[i].position.y) {
            if (pacman.powerup_active) {
                // 고스트 제거
                ghosts[i].position = (Point){rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
            } else {
                game_over = 1;
                playGhostCollisionSound();
                sendLCDMessage(0); // 게임 오버 시 빨간색
                return;
            }
        }
    }

    if (pellets_to_eat <= 0) {
        // 다음 레벨로 이동
        if (level < MAX_LEVEL) {
            level++;
            pellets_to_eat = level * 5; // 레벨마다 더 많은 펠릿을 먹어야 함
            initLevel(); // 다음 레벨을 위한 맵 재설정
            sendLCDMessage(3); // 다음 레벨 시 로고 화면
        } else {
            game_over = 1; // 게임 클리어
            sendLCDMessage(3); // 게임 클리어 시 초기화면
        }
    }

    if (pacman.powerup_active && --pacman.powerup_duration == 0) {
        pacman.powerup_active = 0;
    }
}

void updateGhosts() {
    for (int i = 0; i < 4; i++) {
        Point new_position = ghosts[i].position;
        switch (ghosts[i].direction) {
            case UP:    new_position.y--; break;
            case DOWN:  new_position.y++; break;
            case LEFT:  new_position.x--; break;
            case RIGHT: new_position.x++; break;
        }

        if (!isWall(new_position.x, new_position.y)) {
            ghosts[i].position = new_position;
        } else {
            ghosts[i].direction = rand() % 4;
        }

        if (pacman.position.x == ghosts[i].position.x &&
            pacman.position.y == ghosts[i].position.y) {
            if (pacman.powerup_active) {
                // 고스트 제거
                ghosts[i].position = (Point){rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
            } else {
                game_over = 1;
            }
        }
    }
}