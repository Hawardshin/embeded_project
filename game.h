#ifndef GAME_H
#define GAME_H

#include "utils.h"

#define WIDTH 20
#define HEIGHT 10

#define PELLET_SCORE 10
#define POWERUP_SCORE 50
#define MAX_LEVEL 5
#define MAX_SCORE 999

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point position;
    int direction;
    int powerup_active;
    int powerup_duration;
} Pacman;

typedef struct {
    int x;
    int y;
} Pellet;

typedef struct {
    Point position;
    int direction;
} Ghost;

enum { UP, DOWN, LEFT, RIGHT };

extern Pacman pacman;
extern Pellet pellets[WIDTH * HEIGHT];
extern Pellet powerups[WIDTH * HEIGHT];
extern Ghost ghosts[4];
extern int num_pellets;
extern int num_powerups;
extern int game_over;
extern int score;
extern int level;
extern int pellets_to_eat;

void initGame();
void initLevel();
void updatePacman();
void updateGhosts();
int isWall(int x, int y);
int isPellet(int x, int y);
int isPowerup(int x, int y);
void removePellet(int x, int y);
void removePowerup(int x, int y);

#endif