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

    int screenWidth = 1500;
    int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Catch Game");

    Image background = LoadImage("point.png");
    Texture2D texture = LoadTextureFromImage(background);
    Vector2 position = { 1000,0 };
    Rectangle framerec = { 0.0f,0.0f,(float)500,(float)1000 };

    Image end = LoadImage("end.png");
    Texture2D textureend = LoadTextureFromImage(end);
    Rectangle framerecend = { 0.10f,0.10f,(float)1000,(float)1000 };

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


    Image ghostdead = LoadImage("ghost_dead.png");
    Texture2D texture3dead = LoadTextureFromImage(ghostdead);
    Rectangle framerec3dead = { 0.10f,0.10f,(float)50,(float)50 };

    Ghost* ghost1 = new Ghost(rand() % 1000, rand() % 800);
    Ghost* ghost2 = new Ghost(rand() % 1000, rand() % 800);
    Ghost* ghost3 = new Ghost(rand() % 1000, rand() % 800);
   // Ghost* ghost4 = new Ghost(rand() % 1000, rand() % 800);
   // Ghost* ghost5 = new Ghost(rand() % 1000, rand() % 800);
    //Ghost* ghost6 = new Ghost(rand() % 1000, rand() % 800);
    //Ghost* ghost7 = new Ghost(rand() % 1000, rand() % 800);
    //Ghost* ghost8 = new Ghost(rand() % 1000, rand() % 800);
    //Ghost* ghost9 = new Ghost(rand() % 1000, rand() % 800);

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

    int ghost1dead = 0;
    int ghost2dead = 0;
    int ghost3dead = 0;
    int ghost4dead = 0;
    int ghost5dead = 0;
    int ghost6dead = 0;
    int ghost7dead = 0;
    int ghost8dead = 0;
    int ghost9dead = 0;
    int a[] = {0,0,0,0,0,0,0,0,0};
    int b[] = {200,200,200,200,200,200,200,200,200};
    float ghostdeadposx[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
    float ghostdeadposy[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
    Texture2D p1movetexture = texturestop;
    Rectangle p1moveframe = framerecstop;
    Texture2D p2movetexture = texture2stop;
    Rectangle p2moveframe = framerec2stop;
    Texture2D ghostmovetexture[]={ texture3stop ,texture3stop ,texture3stop,texture3stop ,texture3stop ,texture3stop ,texture3stop ,texture3stop ,texture3stop };
    Rectangle ghostmoveframe[] = { framerec3stop, framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop };

    while (!WindowShouldClose())
    {

        if (ghost1dead == 0) {
            if (b[0] < 0) {
                a[0] = rand() % 4;
                b[0] = 200;
            }
            b[0]--;
            if (a[0] == 0) {
                ghost1->up();
                ghostmovetexture[0] = texture3up;
                ghostmoveframe[0] = framerec3up;
            }
            else if (a[0] == 1) {
                ghost1->down();
                ghostmovetexture[0] = texture3down;
                ghostmoveframe[0] = framerec3down;
            }
            else if (a[0] == 2) {
                ghost1->left();
                ghostmovetexture[0] = texture3left;
                ghostmoveframe[0] = framerec3left;
            }
            else if (a[0] == 3) {
                ghost1->right();
                ghostmovetexture[0] = texture3right;
                ghostmoveframe[0] = framerec3right;
            }

            if (ghost1->x < 0) ghost1->x = 0;
            else if ((ghost1->x + PLAYER_SIZE) > (map.tilesX * MAP_TILE_SIZE)) ghost1->x = (float)map.tilesX * MAP_TILE_SIZE - PLAYER_SIZE;
            if (ghost1->y < 0) ghost1->y = 0;
            else if ((ghost1->y + PLAYER_SIZE) > (map.tilesY * MAP_TILE_SIZE)) ghost1->y = (float)map.tilesY * MAP_TILE_SIZE - PLAYER_SIZE;
        }

        if (ghost2dead == 0) {
            if (b[1] < 0) {
                a[1] = rand() % 4;
                b[1] = 200;
            }
            b[1]--;
            if (a[1] == 0) {
                ghost2->up();
                ghostmovetexture[1] = texture3up;
                ghostmoveframe[1] = framerec3up;
            }
            else if (a[1] == 1) {
                ghost2->down();
                ghostmovetexture[1] = texture3down;
                ghostmoveframe[1] = framerec3down;
            }
            else if (a[1] == 2) {
                ghost2->left();
                ghostmovetexture[1] = texture3left;
                ghostmoveframe[1] = framerec3left;
            }
            else if (a[1] == 3) {
                ghost2->right();
                ghostmovetexture[1] = texture3right;
                ghostmoveframe[1] = framerec3right;
            }

            if (ghost2->x < 0) ghost2->x = 0;
            else if ((ghost2->x + PLAYER_SIZE) > (map.tilesX * MAP_TILE_SIZE)) ghost2->x = (float)map.tilesX * MAP_TILE_SIZE - PLAYER_SIZE;
            if (ghost2->y < 0) ghost2->y = 0;
            else if ((ghost2->y + PLAYER_SIZE) > (map.tilesY * MAP_TILE_SIZE)) ghost2->y = (float)map.tilesY * MAP_TILE_SIZE - PLAYER_SIZE;
        }

        if (ghost3dead == 0) {
            if (b[2] < 0) {
                a[2] = rand() % 4;
                b[2] = 200;
            }
            b[2]--;
            if (a[2] == 0) {
                ghost3->up();
                ghostmovetexture[2] = texture3up;
                ghostmoveframe[2] = framerec3up;
            }
            else if (a[2] == 1) {
                ghost3->down();
                ghostmovetexture[2] = texture3down;
                ghostmoveframe[2] = framerec3down;
            }
            else if (a[2] == 2) {
                ghost3->left();
                ghostmovetexture[2] = texture3left;
                ghostmoveframe[2] = framerec3left;
            }
            else if (a[2] == 3) {
                ghost3->right();
                ghostmovetexture[2] = texture3right;
                ghostmoveframe[2] = framerec3right;
            }

            if (ghost3->x < 0) ghost3->x = 0;
            else if ((ghost3->x + PLAYER_SIZE) > (map.tilesX * MAP_TILE_SIZE)) ghost3->x = (float)map.tilesX * MAP_TILE_SIZE - PLAYER_SIZE;
            if (ghost3->y < 0) ghost3->y = 0;
            else if ((ghost3->y + PLAYER_SIZE) > (map.tilesY * MAP_TILE_SIZE)) ghost3->y = (float)map.tilesY * MAP_TILE_SIZE - PLAYER_SIZE;
        }

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
        DrawTextureRec(texture, framerec, position, WHITE);
        DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        if(ghost1dead==0)
            DrawTextureRec(ghostmovetexture[0], ghostmoveframe[0], {ghost1->x, ghost1->y}, WHITE);
        else
            DrawTextureRec(ghostmovetexture[0], ghostmoveframe[0], { ghostdeadposx[0],ghostdeadposy[0] }, WHITE);
        if (ghost2dead == 0)
            DrawTextureRec(ghostmovetexture[1], ghostmoveframe[1], {ghost2->x, ghost2->y}, WHITE);
        else
            DrawTextureRec(ghostmovetexture[1], ghostmoveframe[1], { ghostdeadposx[1],ghostdeadposy[1] }, WHITE);
        if (ghost3dead == 0)
            DrawTextureRec(ghostmovetexture[2], ghostmoveframe[2], {ghost3->x, ghost3->y}, WHITE);
        else
            DrawTextureRec(ghostmovetexture[2], ghostmoveframe[2], { ghostdeadposx[2],ghostdeadposy[2] }, WHITE);


        DrawTexturePro(fogOfWar.texture, { 0, 0, (float)fogOfWar.texture.width, (float)-fogOfWar.texture.height },
            {
            0, 0, (float)map.tilesX * MAP_TILE_SIZE, (float)map.tilesY * MAP_TILE_SIZE
            },
            {
            0, 0
            }, 0.0f, WHITE);

        if ((firstpeople->x - ghost1->x >= -25 && firstpeople->x - ghost1->x <= 25) && (firstpeople->y - ghost1->y >= -25 && firstpeople->y - ghost1->y <= 25)) {
            firstpeople->point += 5;
            ghostdeadposx[0] = ghost1->x;
            ghostdeadposy[0] = ghost1->y;
            ghostmovetexture[0] = texture3dead;
            ghostmoveframe[0] = framerec3dead;
            ghost1dead = 1;
            delete ghost1;
        }
      
        if ((secondpeople->x - ghost1->x >= -25 && secondpeople->x - ghost1->x <= 25) && (secondpeople->y - ghost1->y >= -25 && secondpeople->y - ghost1->y <= 25)) {
            secondpeople->point += 5;
            ghostdeadposx[0] = ghost1->x;
            ghostdeadposy[0] = ghost1->y;
            ghost1dead = 1;
            ghostmovetexture[0] = texture3dead;
            ghostmoveframe[0] = framerec3dead;
            delete ghost1;
       }


        if ((firstpeople->x - ghost2->x >= -25 && firstpeople->x - ghost2->x <= 25) && (firstpeople->y - ghost2->y >= -25 && firstpeople->y - ghost2->y <= 25)) {
            firstpeople->point += 5;
            ghostdeadposx[1] = ghost2->x;
            ghostdeadposy[1] = ghost2->y;
            ghostmovetexture[1] = texture3dead;
            ghostmoveframe[1] = framerec3dead;
            ghost2dead = 1;
            delete ghost2;
        }

        if ((secondpeople->x - ghost2->x >= -25 && secondpeople->x - ghost2->x <= 25) && (secondpeople->y - ghost2->y >= -25 && secondpeople->y - ghost2->y <= 25)) {
            secondpeople->point += 5;
            ghostdeadposx[1] = ghost2->x;
            ghostdeadposy[1] = ghost2->y;
            ghostmovetexture[1] = texture3dead;
            ghostmoveframe[1] = framerec3dead;
            ghost2dead = 1;
            delete ghost2;
        }

        if ((firstpeople->x - ghost3->x >= -25 && firstpeople->x - ghost3->x <= 25) && (firstpeople->y - ghost3->y >= -25 && firstpeople->y - ghost3->y <= 25)) {
            firstpeople->point += 5;
            ghostdeadposx[2] = ghost3->x;
            ghostdeadposy[2] = ghost3->y;
            ghostmovetexture[2] = texture3dead;
            ghostmoveframe[2] = framerec3dead;
            ghost3dead = 1;
            delete ghost3;
        }

        if ((secondpeople->x - ghost3->x >= -25 && secondpeople->x - ghost3->x <= 25) && (secondpeople->y - ghost3->y >= -25 && secondpeople->y - ghost3->y <= 25)) {
            secondpeople->point += 5;
            ghostdeadposx[2] = ghost3->x;
            ghostdeadposy[2] = ghost3->y;
            ghostmovetexture[2] = texture3dead;
            ghostmoveframe[2] = framerec3dead;
            ghost3dead = 1;
            delete ghost3;
        }

        if (secondpeople->point + firstpeople->point == 15) {
            if (secondpeople->point > firstpeople->point){
              DrawText(TextFormat("Second player wins"), 1000, 300, 20, LIME);
            DrawText(TextFormat("                      Game Finished"), 1000, 400, 20, LIME);
            DrawTextureRec(textureend, framerecend, { 0, 0 }, WHITE);
        }
            else{
            DrawText(TextFormat("First player wins"), 1000, 300, 20, LIME);
            DrawText(TextFormat("                      Game Finished"), 1000, 400, 20, LIME);
            DrawTextureRec(textureend, framerecend, { 0, 0 }, WHITE);
        }
        }
        DrawText(TextFormat("                      Point Table"), 1000, 20, 20, LIME);
        DrawText(TextFormat("The first player point is : %i", firstpeople->point), 1000, 100, 20, LIME);
        DrawText(TextFormat("The second player point is : %i", secondpeople->point), 1000, 200, 20, LIME);
        DrawText(TextFormat("        For exit the game press enter key "), 1000, 500, 20, LIME);
        if (IsKeyDown(KEY_ENTER)) {
            break;
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
