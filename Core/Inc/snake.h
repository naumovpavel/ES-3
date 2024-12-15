#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <stdint.h>

// Game constants
#define SNAKE_MAX_LENGTH 100
#define BLOCK_SIZE 4
#define FIELD_WIDTH (OLED_WIDTH / BLOCK_SIZE)
#define FIELD_HEIGHT ((OLED_HEIGHT - 10) / BLOCK_SIZE)
#define SCORE_HEIGHT 10

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} Direction;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point body[SNAKE_MAX_LENGTH];
    int length;
    Direction dir;
    Point food;
    bool gameOver;
    int score;
} GameState;

extern GameState game;

void initGame(void);
void generateFood(void);
void drawScore(void);
void changeDirection(Direction newDir);
void next(void);

void moveUp(void);
void moveDown(void);
void moveLeft(void);
void moveRight(void);

#endif