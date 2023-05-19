#include "raylib.h"
#include "Player2.hpp"
#include "Player1.hpp"
#include <stdlib.h>    
#include "Ghost.hpp"

#define MAP_TILE_SIZE    50  

typedef struct Map {
    unsigned int tilesX;
    unsigned int tilesY;
    unsigned char* tileIds;
} Map;

int main(void)
{

    int screenWidth = 1000;
    int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Catch Game");

    Map map = { 0 };
    map.tilesX = 20;
    map.tilesY = 20;


    map.tileIds = (unsigned char*)calloc(map.tilesX * map.tilesY, sizeof(unsigned char));


    for (unsigned int i = 0; i < map.tilesY * map.tilesX; i++) map.tileIds[i] = GetRandomValue(0, 1);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        for (unsigned int y = 0; y < map.tilesY; y++)
        {
            for (unsigned int x = 0; x < map.tilesX; x++)
            {
                DrawRectangle(x * MAP_TILE_SIZE, y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE,
                    (map.tileIds[y * map.tilesX + x] == 0) ? Fade(BLACK, 0.8f) : Fade(BLACK, 0.75f));
                DrawRectangleLines(x * MAP_TILE_SIZE, y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE, Fade(BLACK, 0.5f));
            }
        }
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    free(map.tileIds);

    CloseWindow();

    return 0;
}