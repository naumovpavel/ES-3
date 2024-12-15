#include "snake.h"
#include "oled.h"

GameState game;

void intToStr(int num, char *str) {
    int i = 0;
    int isNeg = 0;
    char temp;
    
    if(num < 0) {
        isNeg = 1;
        num = -num;
    }
    
    if(num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    while(num != 0) {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    }
    
    if(isNeg) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    int start = 0;
    int end = i - 1;
    while(start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void initGame(void) {
    game.length = 1;
    game.body[0].x = FIELD_WIDTH / 2;
    game.body[0].y = FIELD_HEIGHT / 2;
    game.dir = RIGHT;
    game.gameOver = 0;
    game.score = 0;
    
    game.food.x = 5;
    game.food.y = 5;
    
    oled_Fill(Black);
    oled_UpdateScreen();
}

void generateFood(void) {
    static uint8_t nextX = 7;
    static uint8_t nextY = 7;
    
    game.food.x = nextX;
    game.food.y = nextY;
    
    nextX = (nextX + 7) % FIELD_WIDTH;
    nextY = (nextY + 5) % FIELD_HEIGHT;
}

void drawScore(void) {
    char scoreStr[12] = "Score: ";
    char numStr[6];
    
    intToStr(game.score, numStr);
    
    int i = 7;
    int j = 0;
    while(numStr[j] != '\0') {
        scoreStr[i++] = numStr[j++];
    }
    scoreStr[i] = '\0';
    
    oled_SetCursor(2, 1);
    oled_WriteString(scoreStr, Font_7x10, White);
}

void changeDirection(Direction newDir) {
    if((game.dir == UP && newDir == DOWN) ||
       (game.dir == DOWN && newDir == UP) ||
       (game.dir == LEFT && newDir == RIGHT) ||
       (game.dir == RIGHT && newDir == LEFT)) {
        return;
    }
    game.dir = newDir;
}

void next(void) {
    if(game.gameOver) {
        oled_Fill(Black);
        oled_SetCursor(30, OLED_HEIGHT/2 - 10);
        oled_WriteString("GAME OVER", Font_7x10, White);
        
        char scoreStr[12] = "Score: ";
        char numStr[6];
        intToStr(game.score, numStr);
        int i = 7;
        int j = 0;
        while(numStr[j] != '\0') {
            scoreStr[i++] = numStr[j++];
        }
        scoreStr[i] = '\0';
        
        oled_SetCursor(30, OLED_HEIGHT/2 + 5);
        oled_WriteString(scoreStr, Font_7x10, White);
        
        oled_UpdateScreen();
        return;
    }

    Point newHead = game.body[0];
    switch(game.dir) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
    }
    
    if(newHead.x < 0 || newHead.x >= FIELD_WIDTH ||
       newHead.y < 0 || newHead.y >= FIELD_HEIGHT) {
        game.gameOver = 1;
        return;
    }
    
    for(int i = 0; i < game.length; i++) {
        if(newHead.x == game.body[i].x && newHead.y == game.body[i].y) {
            game.gameOver = 1;
            return;
        }
    }
    
    for(int i = game.length; i > 0; i--) {
        game.body[i] = game.body[i-1];
    }
    game.body[0] = newHead;
    
    if(newHead.x == game.food.x && newHead.y == game.food.y) {
        game.length++;
        game.score += 10;
        generateFood();
    }
    
    oled_Fill(Black);
    
    drawScore();
    oled_DrawHLine(0, OLED_WIDTH-1, SCORE_HEIGHT, White);
    
    for(int i = 0; i < game.length; i++) {
        oled_DrawSquare(
            game.body[i].x * BLOCK_SIZE,
            (game.body[i].x + 1) * BLOCK_SIZE - 1,
            game.body[i].y * BLOCK_SIZE + SCORE_HEIGHT,
            (game.body[i].y + 1) * BLOCK_SIZE - 1 + SCORE_HEIGHT,
            White
        );
    }
    
    oled_DrawSquare(
        game.food.x * BLOCK_SIZE,
        (game.food.x + 1) * BLOCK_SIZE - 1,
        game.food.y * BLOCK_SIZE + SCORE_HEIGHT,
        (game.food.y + 1) * BLOCK_SIZE - 1 + SCORE_HEIGHT,
        White
    );
    
    oled_UpdateScreen();
}

void moveUp(void) {
    changeDirection(UP);
}

void moveDown(void) {
    changeDirection(DOWN);
}

void moveLeft(void) {
    changeDirection(LEFT);
}

void moveRight(void) {
    changeDirection(RIGHT);
}
