#include <iostream>

#include "raylib.h"

constexpr int windowWidth = 640;
constexpr int windowHeight = 480;

constexpr int ballInitPosX = windowWidth / 2;
constexpr int ballInitPosY = windowHeight / 2;
constexpr int ballRadius = windowWidth / 50;
constexpr int ballBouncingForce = 1;

constexpr int paddleWidth = windowWidth / 40;
constexpr int paddleHeight = windowHeight / 5;

struct Ball {
    int posX;
    int posY;
    int radius;

    int speedDirX;
    int speedDirY;

    void move(int dx, int dy) {
        posX += dx;
        posY += dy;
    }

    void acelerate(int ax, int ay) {
        speedDirX = ax;
        speedDirY = ay;
    }

    void slide() {
        posX += speedDirX;
        posY += speedDirY;
    }

    void bounce(int& side1, int& side2) {
        if (posY + radius >= windowHeight || posY - radius <= 0) {
            speedDirY = -speedDirY;
        }
        if(posX + radius >= windowWidth) {
            side1++;
            posX = ballInitPosX;
            posY = ballInitPosY;
            speedDirX = -speedDirX;
            speedDirY = speedDirY < 0 ? -1 : 1;
        }
        if (posX - radius <= 0) {
            side2++;
            posX = ballInitPosX;
            posY = ballInitPosY;
            speedDirX = -speedDirX;
            speedDirY = speedDirY < 0 ? -1 : 1;
        }
        //if (posX + radius >= windowWidth || posX - radius <= 0) {
        //    speedDirX = -speedDirX;
        //}
    }

    void draw() {
        DrawCircle(posX, posY, radius, WHITE);
    }
};

struct Paddle {
    int posX;
    int posY;

    int width;
    int height;

    int speedDirY;

    void draw() {
        DrawRectangle(posX, posY, width, height, WHITE);
    }

    void move(int dy) {
        posY = dy;
    }

    void acelerate(int ay) {
        speedDirY = ay;
    }

    void slide() {
        posY += speedDirY;
    }

    void collide() {
        if (posY + height > windowHeight) {
            posY = windowHeight - height;
            speedDirY = 0;
        }
        if (posY < 0) {
            posY = 0;
            speedDirY = 0;
        }

    }
};

struct PlayerPaddle : public Paddle {
    void handleKeyInput() {
        if (IsKeyDown(KEY_UP))
            speedDirY--;
        if (IsKeyDown(KEY_DOWN))
            speedDirY++;
    }
};

struct ArtificialPaddle : public Paddle {
    void makeDecision(const Ball& ball) {
        if (posY + height / 2 > ball.posY) {
            speedDirY--;
        }
        if (posY + height / 2 < ball.posY) {
            speedDirY++;
        }
    }
};

int main() {
    int scorePlayer = 0;
    int scoreEnemy = 0;

    Ball ball = {
        ballInitPosX,
        ballInitPosY,
        ballRadius };

    PlayerPaddle paddlePlayer;
    paddlePlayer.posX = 0;
    paddlePlayer.posY = windowHeight / 2 - paddleHeight / 2;
    paddlePlayer.width = paddleWidth;
    paddlePlayer.height = paddleHeight;

    ArtificialPaddle paddleEnemy;
    paddleEnemy.posX = windowWidth - paddleWidth;
    paddleEnemy.posY = windowHeight / 2 - paddleHeight / 2;
    paddleEnemy.width = paddleWidth;
    paddleEnemy.height = paddleHeight;
    paddleEnemy.speedDirY = 0;

    ball.speedDirX = 5;
    ball.speedDirY = 1;

    InitWindow(windowWidth, windowHeight, "Title");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // update
        ball.slide();
        ball.bounce(scorePlayer, scoreEnemy);
        paddlePlayer.handleKeyInput();
        paddlePlayer.slide();
        paddlePlayer.collide();
        paddleEnemy.makeDecision(ball);
        paddleEnemy.slide();
        paddleEnemy.collide();

        if (CheckCollisionCircleRec(
            Vector2{ (float)ball.posX, (float)ball.posY },
            ball.radius,
            Rectangle{ (float)paddlePlayer.posX, (float)paddlePlayer.posY, (float)paddlePlayer.width, (float)paddlePlayer.height })) {

            ball.speedDirX = -ball.speedDirX;
            ball.speedDirY = (ball.speedDirY < 0 ? ball.speedDirY - ballBouncingForce : ball.speedDirY + ballBouncingForce);
        }
        if (CheckCollisionCircleRec(
            Vector2{ (float)ball.posX, (float)ball.posY },
            ball.radius,
            Rectangle{ (float)paddleEnemy.posX, (float)paddleEnemy.posY, (float)paddleEnemy.width, (float)paddleEnemy.height })) {

            ball.speedDirX = -ball.speedDirX;
            ball.speedDirY = (ball.speedDirY < 0 ? ball.speedDirY - ballBouncingForce : ball.speedDirY + ballBouncingForce);
            
        }

        // render
        BeginDrawing();

        // bg
        ClearBackground(BLACK);
        DrawLine(windowWidth / 2, 0, windowWidth / 2, windowHeight, GRAY);
        DrawText(TextFormat("%i", scorePlayer), windowWidth / 2 - 35, windowHeight / 8, 20, GRAY);
        DrawText(TextFormat("%i", scoreEnemy), windowWidth / 2 + 25, windowHeight / 8, 20, GRAY);

        // fg
        ball.draw();

        paddlePlayer.draw();
        paddleEnemy.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
