#define _CRT_SECURE_NO_WARNINGS //to fix visual studio safety checks
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 10

int score = 0;
int gameover = 0;

struct Snake {
    int x, y;
} snake[100];

struct Fruit {
    int x, y;
} fruit;

enum Direction {
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Direction dir;

void generateFruit();
void setup();
void saveHighScore(int currentScore);
int loadHighScore();
void input();
void logic();
void draw();

int main() {
    setup();

    // fix for the cursor bouncing around problem
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    while (!gameover) {
        input();
        logic();
        draw();
        Sleep(75); // refresh rate
    }

    // to stop the fix for the cursor problem after the game ends
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);

    printf("Game Over!\nYour Score: %d\n", score);
    int highScore = loadHighScore();
    printf("High Score: %d\n", highScore);
    saveHighScore(score);

    return 0;
}

void generateFruit() {
    fruit.x = rand() % WIDTH;
    fruit.y = rand() % HEIGHT;
}

void setup() {
    dir = RIGHT;

    // snake genesis
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;

    generateFruit();

    score = 0;
}

void saveHighScore(int currentScore) {
    int highScore = loadHighScore();
    if (currentScore > highScore) {
        FILE* file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", currentScore);
            fclose(file);
        }
    }
}

int loadHighScore() {
    FILE* file = fopen("highscore.txt", "r");
    int highScore = 0;
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    return highScore;
}

//restricts the snake from going backwards

void input() {
    if (_kbhit()) {
        char keyPressed = _getch();
        switch (keyPressed) {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameover = 1;
            break;
        }
    }
}

void logic() {
    int prevX = snake[0].x;
    int prevY = snake[0].y;
    int prev2X, prev2Y;

    switch (dir) {
    case LEFT:
        snake[0].x--;
        break;
    case RIGHT:
        snake[0].x++;
        break;
    case UP:
        snake[0].y--;
        break;
    case DOWN:
        snake[0].y++;
        break;
    }

    // Check for collision with the walls
    if (snake[0].x < 0 || snake[0].x >= WIDTH || snake[0].y < 0 || snake[0].y >= HEIGHT)
        gameover = 1;

    // Check for collision with the food
    if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
        score++;
        generateFruit();
    }

    // Movement the body of the snake
    for (int i = 1; i < score; i++) {
        prev2X = snake[i].x;
        prev2Y = snake[i].y;
        snake[i].x = prevX;
        snake[i].y = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Check for collision with the snakes own body
    for (int i = 1; i < score; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            gameover = 1;
    }
}

void draw() {
    system("cls"); // to clear the console

    // Draw the snake head
    printf("Score: %d\n", score);
    printf("+");
    for (int i = 0; i < WIDTH; i++)
        printf("-");
    printf("+\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0)
                printf("|");

            // Check if the current position is the snake head
            if (i == snake[0].y && j == snake[0].x) {
                printf("O");
            }
            else if (i == fruit.y && j == fruit.x) {
                printf("F");
            }
            else {
                int isBodyPart = 0;
                for (int k = 1; k < score; k++) {
                    if (snake[k].x == j && snake[k].y == i) {
                        printf("o");
                        isBodyPart = 1;
                    }
                }
                if (!isBodyPart)
                    printf(" ");
            }

            if (j == WIDTH - 1)
                printf("|");
        }
        printf("\n");
    }

    printf("+");
    for (int i = 0; i < WIDTH; i++)
        printf("-");
    printf("+\n");
}