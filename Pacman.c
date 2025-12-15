#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#define WIDTH 40
#define HEIGHT 20
int pacmanX, pacmanY;
int score = 0;
int gameOver = 0;
int ghostX[4], ghostY[4];
int ghostDir[4];
char map[HEIGHT][WIDTH];
void initialize();
void draw();
void input();
void logic();
void setCursorPosition(int x, int y);
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void initialize() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                map[i][j] = '#';
            } else if ((i % 4 == 0 && j % 6 == 0) || (i == HEIGHT/2 && j > 10 && j < 30)) {
                map[i][j] = '#';
            } else {
                map[i][j] = '.';
            }
        }
    }
    pacmanX = WIDTH / 2;
    pacmanY = HEIGHT / 2;
    map[pacmanY][pacmanX] = ' ';
    ghostX[0] = 5; ghostY[0] = 5;
    ghostX[1] = WIDTH - 6; ghostY[1] = 5;
    ghostX[2] = 5; ghostY[2] = HEIGHT - 6;
    ghostX[3] = WIDTH - 6; ghostY[3] = HEIGHT - 6;
    for (int i = 0; i < 4; i++) {
        ghostDir[i] = rand() % 4;
        map[ghostY[i]][ghostX[i]] = ' ';
    }
    score = 0;
    gameOver = 0;
}
void draw() {
    setCursorPosition(0, 0);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == pacmanY && j == pacmanX) {
                printf("C");
            } else {
                int isGhost = 0;
                for (int g = 0; g < 4; g++) {
                    if (i == ghostY[g] && j == ghostX[g]) {
                        printf("G");
                        isGhost = 1;
                        break;
                    }
                }
                if (!isGhost) {
                    printf("%c", map[i][j]);
                }
            }
        }
        printf("\n");
    }
    printf("\nScore: %d", score);
    printf("\nUse WASD to move. Press X to quit.\n");
    if (gameOver) {
        printf("\n=== GAME OVER ===\n");
        printf("Final Score: %d\n", score);
    }
}
void input() {
    if (_kbhit()) {
        char key = _getch();
        int newX = pacmanX, newY = pacmanY;
        switch (key) {
            case 'w': case 'W': newY--; break;
            case 's': case 'S': newY++; break;
            case 'a': case 'A': newX--; break;
            case 'd': case 'D': newX++; break;
            case 'x': case 'X': gameOver = 1; return;
        }
        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            if (map[newY][newX] != '#') {
                pacmanX = newX;
                pacmanY = newY;
            }
        }
    }
}
void logic() {
    if (map[pacmanY][pacmanX] == '.') {
        map[pacmanY][pacmanX] = ' ';
        score += 10;
    }
    for (int g = 0; g < 4; g++) {
        if (rand() % 3 == 0) {
            ghostDir[g] = rand() % 4;
        }
        int newGX = ghostX[g], newGY = ghostY[g];
        switch (ghostDir[g]) {
            case 0: newGY--; break;
            case 1: newGY++; break;
            case 2: newGX--; break;
            case 3: newGX++; break;
        }
        if (newGX > 0 && newGX < WIDTH - 1 && newGY > 0 && newGY < HEIGHT - 1) {
            if (map[newGY][newGX] != '#') {
                ghostX[g] = newGX;
                ghostY[g] = newGY;
            } else {
                ghostDir[g] = rand() % 4;
            }
        }
        if (ghostX[g] == pacmanX && ghostY[g] == pacmanY) {
            gameOver = 1;
        }
    }
}
int main() {
    srand(time(NULL));
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    initialize();
    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(100);
    }
    draw();
    printf("\nPress any key to exit...");
    _getch();
    return 0;
}
