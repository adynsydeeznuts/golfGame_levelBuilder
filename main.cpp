#include <iostream>
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Line.h"
#include "Hole.h"
#include "Obstacle.h"

using namespace std;

double lastUpdateTime = 0;

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

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "golf game");
    SetTargetFPS(180);

    Obstacle obstacle("./images/obstacle.png", Vector2{600, 600});
    Obstacle obstacle2("./images/obstacle2.png", Vector2{200, 200});


    const Image backdrop = LoadImage("./images/grass.png");
    Texture2D backdropTexture = LoadTextureFromImage(backdrop);
    UnloadImage(backdrop);

    while (WindowShouldClose() == false){
 
        BeginDrawing();
            DrawTexture(backdropTexture, 0, 0, WHITE);
            obstacle.Draw();
            obstacle.DrawCollisionLines();
            obstacle2.Draw();
            obstacle2.DrawCollisionLines();

        EndDrawing();
    }

    CloseWindow();
}