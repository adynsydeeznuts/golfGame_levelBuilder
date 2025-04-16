#include <iostream>
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Ball.h"
#include "Wall.h"
#include "CurvedWall.h"
#include "Line.h"
#include "Hole.h"
#include "Obstacle.h"

using namespace std;

double lastUpdateTime = 0;

Ball ball(Vector2{540, 480}, Vector2{0.0f, 0.0f}); // Initial position at the center of the screen

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main () {
    const int SCREEN_WIDTH = 1080;
    const int SCREEN_HEIGHT = 960;

    Vector2 startMousePos = {0, 0};
    Vector2 endMousePos = {0, 0};

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "golf game");
    SetTargetFPS(180);

    Obstacle obstacle("./images/obstacle.png", Vector2{600, 600});
    Obstacle obstacle2("./images/obstacle2.png", Vector2{200, 200});


    const Image backdrop = LoadImage("./images/grass.png");
    Texture2D backdropTexture = LoadTextureFromImage(backdrop);
    UnloadImage(backdrop);

    while (WindowShouldClose() == false){

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            startMousePos = GetMousePosition();
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            endMousePos = GetMousePosition();
            ball.Hit(startMousePos, endMousePos);

        }
        ball.updatePosition();

        for(size_t i = 0; i < obstacle.GetCollisionLines().size(); i++) {
                ball.CheckCollision(obstacle.GetCollisionLines()[i]);
        }
        for(size_t i = 0; i < obstacle2.GetCollisionLines().size(); i++) {
            ball.CheckCollision(obstacle2.GetCollisionLines()[i]);
    }
 
        BeginDrawing();
            DrawTexture(backdropTexture, 0, 0, WHITE);
            ball.Draw();
            obstacle.Draw();
            obstacle.DrawCollisionLines();
            obstacle2.Draw();
            obstacle2.DrawCollisionLines();

        EndDrawing();
    }

    CloseWindow();
}