#include "raylib.h"
#include "Player2.hpp"
#include "Player1.hpp"
#include <stdlib.h>    
#include "Ghost.hpp"

#define MAP_TILE_SIZE    50  
#define PLAYER_SIZE    50
#define PLAYER_TILE_VISIBILITY  2

typedef struct Map {
    unsigned int tilesX;
    unsigned int tilesY;
    unsigned char* tileIds;
    unsigned char* tileFog;
} Map;

int main(void)
{

    int screenWidth = 1000;
    int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Catch Game");

    Player2* secondpeople = new Player2;
    Player1* firstpeople = new Player1;

    Image pl1stop = LoadImage("boy_stop_1.png");
    Texture2D texturestop = LoadTextureFromImage(pl1stop);
    Rectangle framerecstop = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1up = LoadImage("boy_up_1.png");
    Texture2D textureup = LoadTextureFromImage(pl1up);
    Rectangle framerecup = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1down = LoadImage("boy_down_1.png");
    Texture2D texturedown = LoadTextureFromImage(pl1down);
    Rectangle framerecdown = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1right = LoadImage("boy_right_1.png");
    Texture2D textureright = LoadTextureFromImage(pl1right);
    Rectangle framerecright = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1left = LoadImage("boy_left_1.png");
    Texture2D textureleft = LoadTextureFromImage(pl1left);
    Rectangle framerecleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2stop = LoadImage("boy_stop_2.png");
    Texture2D texture2stop = LoadTextureFromImage(pl2stop);
    Rectangle framerec2stop = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2up = LoadImage("boy_up_2.png");
    Texture2D texture2up = LoadTextureFromImage(pl2up);
    Rectangle framerec2up = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2down = LoadImage("boy_down_2.png");
    Texture2D texture2down = LoadTextureFromImage(pl2down);
    Rectangle framerec2down = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2left = LoadImage("boy_left_2.png");
    Texture2D texture2left = LoadTextureFromImage(pl2left);
    Rectangle framerec2left = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2right = LoadImage("boy_right_2.png");
    Texture2D texture2right = LoadTextureFromImage(pl2right);
    Rectangle framerec2right = { 0.10f,0.10f,(float)50,(float)50 };

    Ghost* ghost1 = new Ghost(400, 100);
    Image ghoststop = LoadImage("ghost_stop.png");
    Texture2D texture3stop = LoadTextureFromImage(ghoststop);
    Rectangle framerec3stop = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostup = LoadImage("ghost_up.png");
    Texture2D texture3up = LoadTextureFromImage(ghostup);
    Rectangle framerec3up = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostdown = LoadImage("ghost_down.png");
    Texture2D texture3down = LoadTextureFromImage(ghostdown);
    Rectangle framerec3down = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostleft = LoadImage("ghost_left.png");
    Texture2D texture3left = LoadTextureFromImage(ghostleft);
    Rectangle framerec3left = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostright = LoadImage("ghost_stop.png");
    Texture2D texture3right = LoadTextureFromImage(ghostright);
    Rectangle framerec3right = { 0.10f,0.10f,(float)50,(float)50 };



    Map map = { 0 };
    map.tilesX = 20;
    map.tilesY = 20;


    map.tileIds = (unsigned char*)calloc(map.tilesX * map.tilesY, sizeof(unsigned char));
    map.tileFog = (unsigned char*)calloc(map.tilesX * map.tilesY, sizeof(unsigned char));

    for (unsigned int i = 0; i < map.tilesY * map.tilesX; i++) map.tileIds[i] = GetRandomValue(0, 1);

    Vector2 playerPosition = { 180, 130 };
    int player1TileX = 0;
    int player1TileY = 0;

    int player2TileX = 0;
    int player2TileY = 0;

    int ghostTileX = 0;
    int ghostTileY = 0;

    RenderTexture2D fogOfWar = LoadRenderTexture(map.tilesX, map.tilesY);
    SetTextureFilter(fogOfWar.texture, TEXTURE_FILTER_BILINEAR);

    SetTargetFPS(60);

    int a = 0;
    int b = 200;
    Texture2D p1movetexture = texturestop;
    Rectangle p1moveframe = framerecstop;
    Texture2D p2movetexture = texture2stop;
    Rectangle p2moveframe = framerec2stop;
    Texture2D ghostmovetexture = texture3stop;
    Rectangle ghostmoveframe = framerec3stop;

    while (!WindowShouldClose())
    {
        if (b < 0) {
            a = rand() % 4;
            b = 200;
        }
        b--;
        if (a == 0) {
            ghost1->up();
            ghostmovetexture = texture3up;
            ghostmoveframe = framerec3up;
        }
        else if (a == 1) {
            ghost1->down();
            ghostmovetexture = texture3down;
            ghostmoveframe = framerec3down;
        }
        else if (a == 2) {
            ghost1->left();
            ghostmovetexture = texture3left;
            ghostmoveframe = framerec3left;
        }
        else if (a == 3) {
            ghost1->right();
            ghostmovetexture = texture3right;
            ghostmoveframe = framerec3right;
        }

        if (ghost1->x < 0) ghost1->x = 0;
        else if ((ghost1->x + PLAYER_SIZE) > (map.tilesX * MAP_TILE_SIZE)) ghost1->x = (float)map.tilesX * MAP_TILE_SIZE - PLAYER_SIZE;
        if (ghost1->y < 0) ghost1->y = 0;
        else if ((ghost1->y + PLAYER_SIZE) > (map.tilesY * MAP_TILE_SIZE)) ghost1->y = (float)map.tilesY * MAP_TILE_SIZE - PLAYER_SIZE;


        if (IsKeyDown(KEY_RIGHT)) {
            firstpeople->right();
            p1movetexture = textureright;
            p1moveframe = framerecright;
        }
        else
            if (IsKeyDown(KEY_LEFT)) {
                firstpeople->left();
                p1movetexture = textureleft;
                p1moveframe = framerecleft;
            }
            else
                if (IsKeyDown(KEY_UP)) {
                    firstpeople->up();
                    p1movetexture = textureup;
                    p1moveframe = framerecup;
                }
                else
                    if (IsKeyDown(KEY_DOWN)) {
                        firstpeople->down();
                        p1movetexture = texturedown;
                        p1moveframe = framerecdown;
                    }
                    else {
                        p1movetexture = texturestop;
                        p1moveframe = framerecstop;
                    }

        if (firstpeople->x < 0) firstpeople->x = 0;
        else if ((firstpeople->x + PLAYER_SIZE) > (map.tilesX * MAP_TILE_SIZE)) firstpeople->x = (float)map.tilesX * MAP_TILE_SIZE - PLAYER_SIZE;
        if (firstpeople->y < 0) firstpeople->y = 0;
        else if ((firstpeople->y + PLAYER_SIZE) > (map.tilesY * MAP_TILE_SIZE)) firstpeople->y = (float)map.tilesY * MAP_TILE_SIZE - PLAYER_SIZE;

        if (IsKeyDown(KEY_D)) {
            secondpeople->right();
            p2movetexture = texture2right;
            p2moveframe = framerec2right;

        }
        else if (IsKeyDown(KEY_A)) {
            secondpeople->left();
            p2movetexture = texture2left;
            p2moveframe = framerec2left;

        }
        else if (IsKeyDown(KEY_S)) {
            secondpeople->down();
            p2movetexture = texture2down;
            p2moveframe = framerec2down;
        }
        else if (IsKeyDown(KEY_W)) {
            secondpeople->up();
            p2movetexture = texture2up;
            p2moveframe = framerec2up;
        }
        else
        {
            p2movetexture = texture2stop;
            p2moveframe = framerec2stop;
        }

        if (secondpeople->x < 0) secondpeople->x = 0;
        else if ((secondpeople->x + PLAYER_SIZE) > (map.tilesX * MAP_TILE_SIZE)) secondpeople->x = (float)map.tilesX * MAP_TILE_SIZE - PLAYER_SIZE;
        if (secondpeople->y < 0) secondpeople->y = 0;
        else if ((secondpeople->y + PLAYER_SIZE) > (map.tilesY * MAP_TILE_SIZE)) secondpeople->y = (float)map.tilesY * MAP_TILE_SIZE - PLAYER_SIZE;

        for (unsigned int i = 0; i < map.tilesX * map.tilesY; i++) if (map.tileFog[i] == 1) map.tileFog[i] = 2;

        player1TileX = (int)((firstpeople->x + MAP_TILE_SIZE / 2) / MAP_TILE_SIZE);
        player1TileY = (int)((firstpeople->y + MAP_TILE_SIZE / 2) / MAP_TILE_SIZE);

        player2TileX = (int)((secondpeople->x + MAP_TILE_SIZE / 2) / MAP_TILE_SIZE);
        player2TileY = (int)((secondpeople->y + MAP_TILE_SIZE / 2) / MAP_TILE_SIZE);

        for (int y = (player1TileY - PLAYER_TILE_VISIBILITY); y < (player1TileY + PLAYER_TILE_VISIBILITY); y++)
            for (int x = (player1TileX - PLAYER_TILE_VISIBILITY); x < (player1TileX + PLAYER_TILE_VISIBILITY); x++)
                if ((x >= 0) && (x < (int)map.tilesX) && (y >= 0) && (y < (int)map.tilesY)) map.tileFog[y * map.tilesX + x] = 1;

        for (int y = (player2TileY - PLAYER_TILE_VISIBILITY); y < (player2TileY + PLAYER_TILE_VISIBILITY); y++)
            for (int x = (player2TileX - PLAYER_TILE_VISIBILITY); x < (player2TileX + PLAYER_TILE_VISIBILITY); x++)
                if ((x >= 0) && (x < (int)map.tilesX) && (y >= 0) && (y < (int)map.tilesY)) map.tileFog[y * map.tilesX + x] = 1;

        BeginTextureMode(fogOfWar);
        ClearBackground(BLANK);
        for (unsigned int y = 0; y < map.tilesY; y++)
            for (unsigned int x = 0; x < map.tilesX; x++)
                if (map.tileFog[y * map.tilesX + x] == 0) DrawRectangle(x, y, 1, 1, BLACK);
                else if (map.tileFog[y * map.tilesX + x] == 2) DrawRectangle(x, y, 1, 1, BLACK);
        EndTextureMode();

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
        DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        DrawTextureRec(ghostmovetexture, ghostmoveframe, { ghost1->x, ghost1->y }, WHITE);

        DrawTexturePro(fogOfWar.texture, { 0, 0, (float)fogOfWar.texture.width, (float)-fogOfWar.texture.height },
            {
            0, 0, (float)map.tilesX * MAP_TILE_SIZE, (float)map.tilesY * MAP_TILE_SIZE
            },
            {
            0, 0
            }, 0.0f, WHITE);

        if ((firstpeople->x - ghost1->x >= -25 && firstpeople->x - ghost1->x <= 25) && (firstpeople->y - ghost1->y >= -25 && firstpeople->y - ghost1->y <= 25)) {
            firstpeople->point += 5;
            int x1 = ghost1->x;
            int y1 = ghost1->y;

            delete ghost1;

            Ghost* ghost1 = new Ghost(rand() % 1000, rand() % 800);
        }

        if ((secondpeople->x - ghost1->x >= -25 && secondpeople->x - ghost1->x <= 25) && (secondpeople->y - ghost1->y >= -25 && secondpeople->y - ghost1->y <= 25)) {
            secondpeople->point += 5;
            int x2 = ghost1->x + 1;
            int y2 = ghost1->y + 1;
            delete ghost1;
            Ghost* ghost1 = new Ghost(rand() % 1000, rand() % 800);
        }

        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    free(map.tileIds);
    free(map.tileFog);

    UnloadRenderTexture(fogOfWar);
    CloseWindow();

    return 0;
}
