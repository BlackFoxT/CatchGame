#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "Player1.hpp"
#include "Player2.hpp"
#include "Ghost.hpp"
#include <stdlib.h>   

#define RLGL_SRC_ALPHA 0x0302
#define RLGL_MIN 0x8007
#define RLGL_MAX 0x8008
#define MAP_TILE_SIZE    50  
#define PLAYER_SIZE    50
#define MAX_BOXES     20
#define MAX_SHADOWS   MAX_BOXES 
#define MAX_LIGHTS    16

typedef struct ShadowGeometry {
    Vector2 vertices1[4];
} ShadowGeometry;

typedef struct LightInfo {
    bool active;
    bool dirty;
    bool valid;  
    Vector2 position;
    RenderTexture mask;  
    float outerRadius;  
    Rectangle bounds;  
    ShadowGeometry shadows[MAX_SHADOWS];
    int shadowCount;
} LightInfo;

LightInfo lights1[MAX_LIGHTS] = { 0 };
LightInfo lights2[MAX_LIGHTS] = { 0 };
LightInfo lights3[MAX_LIGHTS] = { 0 };

void MoveLight(int slot, float x, float y, LightInfo lights[MAX_LIGHTS])
{
    lights[slot].dirty = true;
    lights[slot].position.x = x;
    lights[slot].position.y = y;
    lights[slot].bounds.x = x - lights[slot].outerRadius;
    lights[slot].bounds.y = y - lights[slot].outerRadius;

}

void ComputeShadowVolumeForEdge(int slot, Vector2 sp, Vector2 ep, LightInfo lights[MAX_LIGHTS])
{
    if (lights[slot].shadowCount >= MAX_SHADOWS) return;

    float extension = lights[slot].outerRadius * 2;

    Vector2 spVector = Vector2Normalize(Vector2Subtract(sp, lights[slot].position));
    Vector2 spProjection = Vector2Add(sp, Vector2Scale(spVector, extension));
    Vector2 epVector = Vector2Normalize(Vector2Subtract(ep, lights1[slot].position));
    Vector2 epProjection = Vector2Add(ep, Vector2Scale(epVector, extension));

    lights1[slot].shadows[lights1[slot].shadowCount].vertices1[0] = sp;
    lights1[slot].shadows[lights1[slot].shadowCount].vertices1[1] = ep;
    lights1[slot].shadows[lights1[slot].shadowCount].vertices1[2] = epProjection;
    lights1[slot].shadows[lights1[slot].shadowCount].vertices1[3] = spProjection;

}

void DrawLightMask(int slot, LightInfo lights[MAX_LIGHTS])
{
    BeginTextureMode(lights[slot].mask);
    ClearBackground(WHITE);
    rlSetBlendFactors(RLGL_SRC_ALPHA, RLGL_SRC_ALPHA, RLGL_MIN);
    rlSetBlendMode(BLEND_CUSTOM);

   if (lights[slot].valid) DrawCircleGradient((int)lights[slot].position.x, (int)lights[slot].position.y, lights[slot].outerRadius, ColorAlpha(WHITE, 0), WHITE);

    rlSetBlendMode(BLEND_ALPHA);
    rlSetBlendMode(BLEND_CUSTOM);
    rlSetBlendMode(BLEND_ALPHA);

    EndTextureMode();
}

void SetupLight(int slot, float x, float y, float radius, LightInfo lights[MAX_LIGHTS])
{
    lights[slot].active = true;
    lights[slot].valid = false;
    lights[slot].mask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    lights[slot].outerRadius = radius;
    lights[slot].bounds.width = radius * 2;
    lights[slot].bounds.height = radius * 2;

    MoveLight(slot, x, y, lights);
    DrawLightMask(slot, lights);
}

bool UpdateLight(int slot, Rectangle* boxes, int count, LightInfo lights[MAX_LIGHTS])
{
    if (!lights[slot].active || !lights[slot].dirty) return false;

    lights[slot].dirty = false;
    lights[slot].shadowCount = 0;
    lights[slot].valid = false;

    for (int i = 0; i < count; i++)
    {
        if (CheckCollisionPointRec(lights[slot].position, boxes[i])) return false;

        if (!CheckCollisionRecs(lights[slot].bounds, boxes[i])) continue;

        Vector2 sp = { boxes[i].x, boxes[i].y };
        Vector2 ep = { boxes[i].x + boxes[i].width, boxes[i].y };

        if (lights[slot].position.y > ep.y) ComputeShadowVolumeForEdge(slot, sp, ep, lights);

        sp = ep;
        ep.y += boxes[i].height;
        if (lights[slot].position.x < ep.x) ComputeShadowVolumeForEdge(slot, sp, ep, lights);

        sp = ep;
        ep.x -= boxes[i].width;
        if (lights[slot].position.y < ep.y) ComputeShadowVolumeForEdge(slot, sp, ep, lights);

        sp = ep;
        ep.y -= boxes[i].height;
        if (lights[slot].position.x > ep.x) ComputeShadowVolumeForEdge(slot, sp, ep, lights);

        lights[slot].shadows[lights[slot].shadowCount].vertices1[0] = { boxes[i].x, boxes[i].y };
        lights[slot].shadows[lights[slot].shadowCount].vertices1[1] = { boxes[i].x, boxes[i].y + boxes[i].height };
        lights[slot].shadows[lights[slot].shadowCount].vertices1[2] = { boxes[i].x + boxes[i].width, boxes[i].y + boxes[i].height };
        lights[slot].shadows[lights[slot].shadowCount].vertices1[3] = { boxes[i].x + boxes[i].width, boxes[i].y };
        lights[slot].shadowCount++;
    }

    lights[slot].valid = true;

    DrawLightMask(slot, lights);

    return true;
}

int main(void)
{
    const int screenWidth = 1500;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Catch game");
    Player1* firstpeople = new Player1;
    Player2* secondpeople = new Player2;

    Image background = LoadImage("dark.png");
    Texture2D backgroundtexture = LoadTextureFromImage(background);
    Rectangle backgroundframerec = { 0.0f,0.0f,(float)1000,(float)1000 };

    Image point = LoadImage("point.png");
    Texture2D texture = LoadTextureFromImage(point);
    Vector2 position = { 1000,0 };
    Rectangle framerec = { 0.0f,0.0f,(float)500,(float)1000 };

    Image end = LoadImage("end.png");
    Texture2D textureend = LoadTextureFromImage(end);
    Rectangle framerecend = { 0.10f,0.10f,(float)1000,(float)1000 };

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

    Image ghostright = LoadImage("ghost_right.png");
    Texture2D texture3right = LoadTextureFromImage(ghostright);
    Rectangle framerec3right = { 0.10f,0.10f,(float)50,(float)50 };


    Image ghostdead = LoadImage("ghost_dead.png");
    Texture2D texture3dead = LoadTextureFromImage(ghostdead);
    Rectangle framerec3dead = { 0.10f,0.10f,(float)50,(float)50 };

    Ghost* ghost1 = new Ghost(rand() % 1000, rand() % 1000);
    Ghost* ghost2 = new Ghost(rand() % 1000, rand() % 1000);
    Ghost* ghost3 = new Ghost(rand() % 1000, rand() % 1000);
    Ghost* ghost4 = new Ghost(rand() % 1000, rand() % 1000);
    Ghost* ghost5 = new Ghost(rand() % 1000, rand() % 1000);
    Ghost* ghost6 = new Ghost(-100, -100);
    Ghost* ghost7 = new Ghost(-100, -100);
    Ghost* ghost8 = new Ghost(-100, -100);
    Ghost* ghost9 = new Ghost(-100, -100);
    Ghost* ghost10 = new Ghost(-100, -100);
    
    int boxCount = 0;
    Rectangle boxes[MAX_BOXES] = { 0 };

    RenderTexture lightMask = LoadRenderTexture(1000, 1000);

    SetupLight(0, 200, 400, 100, lights1);
    SetupLight(0, 100, 400, 100, lights2);
   
    bool showLines = false;

    SetTargetFPS(60);  
    int ghost1dead = 0;
    int ghost2dead = 0;
    int ghost3dead = 0;
    int ghost4dead = 0;
    int ghost5dead = 0;
    int ghost6dead = 1;
    int ghost7dead = 1;
    int ghost8dead = 1;
    int ghost9dead = 1;
    int ghost10dead = 1;

    int ghostdirections[] = { rand() % 4,rand() % 4,rand() % 4,rand() % 4,rand() % 4,rand() % 4,rand() % 4,rand() % 4,rand() % 4,rand() % 4 };
    int steps[] = { 200,200,200,200,200,200,200,200,200,200 };
    float ghostdeadposx[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
    float ghostdeadposy[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

    Texture2D p1movetexture = texturestop;
    Rectangle p1moveframe = framerecstop;
    Texture2D p2movetexture = texture2stop;
    Rectangle p2moveframe = framerec2stop;
    Texture2D ghostmovetexture[] = { texture3stop ,texture3stop ,texture3stop,texture3stop ,texture3stop ,texture3stop ,texture3stop ,texture3stop ,texture3stop,texture3stop };
    Rectangle ghostmoveframe[] = { framerec3stop, framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop,framerec3stop };

    while (!WindowShouldClose())
    {
       if (ghost1dead == 0) {
            if (steps[0] < 0) {
                ghostdirections[0] = rand() % 4;
                steps[0] = 200;
            }
            steps[0]--;
            if (ghostdirections[0] == 0) {
                ghost1->up();
                ghostmovetexture[0] = texture3up;
                ghostmoveframe[0] = framerec3up;
            }
            else if (ghostdirections[0] == 1) {
                ghost1->down();
                ghostmovetexture[0] = texture3down;
                ghostmoveframe[0] = framerec3down;
            }
            else if (ghostdirections[0] == 2) {
                ghost1->left();
                ghostmovetexture[0] = texture3left;
                ghostmoveframe[0] = framerec3left;
            }
            else if (ghostdirections[0] == 3) {
                ghost1->right();
                ghostmovetexture[0] = texture3right;
                ghostmoveframe[0] = framerec3right;
            }

          }

        if (ghost2dead == 0) {
            if (steps[1] < 0) {
                ghostdirections[1] = rand() % 4;
                steps[1] = 200;
            }
            steps[1]--;
            if (ghostdirections[1] == 0) {
                ghost2->up();
                ghostmovetexture[1] = texture3up;
                ghostmoveframe[1] = framerec3up;
            }
            else if (ghostdirections[1] == 1) {
                ghost2->down();
                ghostmovetexture[1] = texture3down;
                ghostmoveframe[1] = framerec3down;
            }
            else if (ghostdirections[1] == 2) {
                ghost2->left();
                ghostmovetexture[1] = texture3left;
                ghostmoveframe[1] = framerec3left;
            }
            else if (ghostdirections[1] == 3) {
                ghost2->right();
                ghostmovetexture[1] = texture3right;
                ghostmoveframe[1] = framerec3right;
            }

         }

        if (ghost3dead == 0) {
            if (steps[2] < 0) {
                ghostdirections[2] = rand() % 4;
                steps[2] = 200;
            }
            steps[2]--;
            if (ghostdirections[2] == 0) {
                ghost3->up();
                ghostmovetexture[2] = texture3up;
                ghostmoveframe[2] = framerec3up;
            }
            else if (ghostdirections[2] == 1) {
                ghost3->down();
                ghostmovetexture[2] = texture3down;
                ghostmoveframe[2] = framerec3down;
            }
            else if (ghostdirections[2] == 2) {
                ghost3->left();
                ghostmovetexture[2] = texture3left;
                ghostmoveframe[2] = framerec3left;
            }
            else if (ghostdirections[2] == 3) {
                ghost3->right();
                ghostmovetexture[2] = texture3right;
                ghostmoveframe[2] = framerec3right;
            }

         }

        if (ghost4dead == 0) {
            if (steps[3] < 0) {
                ghostdirections[3] = rand() % 4;
                steps[3] = 200;
            }
            steps[3]--;
            if (ghostdirections[3] == 0) {
                ghost4->up();
                ghostmovetexture[3] = texture3up;
                ghostmoveframe[3] = framerec3up;
            }
            else if (ghostdirections[3] == 1) {
                ghost4->down();
                ghostmovetexture[3] = texture3down;
                ghostmoveframe[3] = framerec3down;
            }
            else if (ghostdirections[3] == 2) {
                ghost4->left();
                ghostmovetexture[3] = texture3left;
                ghostmoveframe[3] = framerec3left;
            }
            else if (ghostdirections[3] == 3) {
                ghost4->right();
                ghostmovetexture[3] = texture3right;
                ghostmoveframe[3] = framerec3right;
            }

         }

        if (ghost5dead == 0) {
            if (steps[4] < 0) {
                ghostdirections[4] = rand() % 4;
                steps[4] = 200;
            }
            steps[4]--;
            if (ghostdirections[4] == 0) {
                ghost5->up();
                ghostmovetexture[4] = texture3up;
                ghostmoveframe[4] = framerec3up;
            }
            else if (ghostdirections[4] == 1) {
                ghost5->down();
                ghostmovetexture[4] = texture3down;
                ghostmoveframe[4] = framerec3down;
            }
            else if (ghostdirections[4] == 2) {
                ghost5->left();
                ghostmovetexture[4] = texture3left;
                ghostmoveframe[4] = framerec3left;
            }
            else if (ghostdirections[4] == 3) {
                ghost5->right();
                ghostmovetexture[4] = texture3right;
                ghostmoveframe[4] = framerec3right;
            }

          }

        if (ghost6dead == 0) {
            if (steps[5] < 0) {
                ghostdirections[5] = rand() % 4;
                steps[5] = 200;
            }
            steps[5]--;
            if (ghostdirections[5] == 0) {
                ghost6->up();
                ghostmovetexture[5] = texture3up;
                ghostmoveframe[5] = framerec3up;
            }
            else if (ghostdirections[5] == 1) {
                ghost6->down();
                ghostmovetexture[5] = texture3down;
                ghostmoveframe[5] = framerec3down;
            }
            else if (ghostdirections[5] == 2) {
                ghost6->left();
                ghostmovetexture[5] = texture3left;
                ghostmoveframe[5] = framerec3left;
            }
            else if (ghostdirections[5] == 3) {
                ghost6->right();
                ghostmovetexture[5] = texture3right;
                ghostmoveframe[5] = framerec3right;
            }
         }

        if (ghost7dead == 0) {
            if (steps[6] < 0) {
                ghostdirections[6] = rand() % 4;
                steps[6] = 200;
            }
            steps[6]--;
            if (ghostdirections[6] == 0) {
                ghost7->up();
                ghostmovetexture[6] = texture3up;
                ghostmoveframe[6] = framerec3up;
            }
            else if (ghostdirections[6] == 1) {
                ghost7->down();
                ghostmovetexture[6] = texture3down;
                ghostmoveframe[6] = framerec3down;
            }
            else if (ghostdirections[6] == 2) {
                ghost7->left();
                ghostmovetexture[6] = texture3left;
                ghostmoveframe[6] = framerec3left;
            }
            else if (ghostdirections[6] == 3) {
                ghost7->right();
                ghostmovetexture[6] = texture3right;
                ghostmoveframe[6] = framerec3right;
            }
        }

        if (ghost8dead == 0) {
            if (steps[7] < 0) {
                ghostdirections[7] = rand() % 4;
                steps[7] = 200;
            }
            steps[7]--;
            if (ghostdirections[7] == 0) {
                ghost8->up();
                ghostmovetexture[7] = texture3up;
                ghostmoveframe[7] = framerec3up;
            }
            else if (ghostdirections[7] == 1) {
                ghost8->down();
                ghostmovetexture[7] = texture3down;
                ghostmoveframe[7] = framerec3down;
            }
            else if (ghostdirections[7] == 2) {
                ghost8->left();
                ghostmovetexture[7] = texture3left;
                ghostmoveframe[7] = framerec3left;
            }
            else if (ghostdirections[7] == 3) {
                ghost8->right();
                ghostmovetexture[7] = texture3right;
                ghostmoveframe[7] = framerec3right;
            }
        }

        if (ghost9dead == 0) {
            if (steps[8] < 0) {
                ghostdirections[8] = rand() % 4;
                steps[8] = 200;
            }
            steps[8]--;
            if (ghostdirections[8] == 0) {
                ghost9->up();
                ghostmovetexture[8] = texture3up;
                ghostmoveframe[8] = framerec3up;
            }
            else if (ghostdirections[8] == 1) {
                ghost9->down();
                ghostmovetexture[8] = texture3down;
                ghostmoveframe[8] = framerec3down;
            }
            else if (ghostdirections[8] == 2) {
                ghost9->left();
                ghostmovetexture[8] = texture3left;
                ghostmoveframe[8] = framerec3left;
            }
            else if (ghostdirections[8] == 3) {
                ghost9->right();
                ghostmovetexture[8] = texture3right;
                ghostmoveframe[8] = framerec3right;
            }
        }

        if (ghost10dead == 0) {
            if (steps[9] < 0) {
                ghostdirections[9] = rand() % 4;
                steps[9] = 200;
            }
            steps[9]--;
            if (ghostdirections[9] == 0) {
                ghost10->up();
                ghostmovetexture[9] = texture3up;
                ghostmoveframe[9] = framerec3up;
            }
            else if (ghostdirections[9] == 1) {
                ghost10->down();
                ghostmovetexture[9] = texture3down;
                ghostmoveframe[9] = framerec3down;
            }
            else if (ghostdirections[9] == 2) {
                ghost10->left();
                ghostmovetexture[9] = texture3left;
                ghostmoveframe[9] = framerec3left;
            }
            else if (ghostdirections[9] == 3) {
                ghost10->right();
                ghostmovetexture[9] = texture3right;
                ghostmoveframe[9] = framerec3right;
            }
        }

        if (IsKeyDown(KEY_RIGHT)) {
            firstpeople->right();
            p1movetexture = textureright;
            p1moveframe = framerecright;
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else
            if (IsKeyDown(KEY_LEFT)) {
                firstpeople->left();
                p1movetexture = textureleft;
                p1moveframe = framerecleft;
                MoveLight(0, firstpeople->x, firstpeople->y, lights1);
                DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
            }
            else
                if (IsKeyDown(KEY_UP)) {
                    firstpeople->up();
                    p1movetexture = textureup;
                    p1moveframe = framerecup;
                    MoveLight(0, firstpeople->x, firstpeople->y, lights1);
                    DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
                }
                else
                    if (IsKeyDown(KEY_DOWN)) {
                        firstpeople->down();
                        p1movetexture = texturedown;
                        p1moveframe = framerecdown;
                        MoveLight(0, firstpeople->x, firstpeople->y, lights1);
                        DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
                    }
                    else {
                        p1movetexture = texturestop;
                        p1moveframe = framerecstop;
                        MoveLight(0, firstpeople->x, firstpeople->y, lights1);
                        DrawTextureRec(p1movetexture, p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
                    }

        if (IsKeyDown(KEY_D)) {
            secondpeople->right();
            p2movetexture = texture2right;
            p2moveframe = framerec2right;
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_A)) {
            secondpeople->left();
            p2movetexture = texture2left;
            p2moveframe = framerec2left;
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_S)) {
            secondpeople->down();
            p2movetexture = texture2down;
            p2moveframe = framerec2down;
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_W)) {
            secondpeople->up();
            p2movetexture = texture2up;
            p2moveframe = framerec2up;
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else
        {
            p2movetexture = texture2stop;
            p2moveframe = framerec2stop;
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(p2movetexture, p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }

        if (IsKeyPressed(KEY_F1)) showLines = !showLines;

        bool dirtyLights1 = false;
        bool dirtyLights2 = false;
        for (int i = 0; i < MAX_LIGHTS; i++){
            if (UpdateLight(i, boxes, boxCount, lights1)) dirtyLights1 = true;
            if (UpdateLight(i, boxes, boxCount, lights2)) dirtyLights2 = true;
        }

        BeginTextureMode(lightMask);
        ClearBackground(BLACK);

        rlSetBlendMode(BLEND_CUSTOM);

        for (int i = 0; i < MAX_LIGHTS; i++){
            if (lights1[i].active) DrawTextureRec(lights1[i].mask.texture, { 0, 0, (float)1000, -(float)1000 }, Vector2Zero(), WHITE);
            if (lights2[i].active) DrawTextureRec(lights2[i].mask.texture, { 0, 0, (float)1000, -(float)1000 }, Vector2Zero(), WHITE);
        }

        rlSetBlendMode(BLEND_ALPHA);
        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK);

        DrawTextureRec(backgroundtexture, backgroundframerec, { 0,0 }, WHITE);
        DrawTextureRec(texture, framerec, position, WHITE);

        for (int i = 0; i < MAX_LIGHTS; i++){
            if (lights1[i].active) DrawTextureRec(p1movetexture, p1moveframe, { (float)lights1[i].position.x, (float)lights1[i].position.y }, WHITE);
            if (lights2[i].active) DrawTextureRec(p2movetexture, p2moveframe, { (float)lights2[i].position.x, (float)lights2[i].position.y }, WHITE);
            if (ghost1dead == 0)
                DrawTextureRec(ghostmovetexture[0], ghostmoveframe[0], { ghost1->x, ghost1->y }, WHITE);
            else if (ghost1dead == 2)
                DrawTextureRec(ghostmovetexture[0], ghostmoveframe[0], { ghostdeadposx[0],ghostdeadposy[0] }, WHITE);
            if (ghost2dead == 0)
                DrawTextureRec(ghostmovetexture[1], ghostmoveframe[1], { ghost2->x, ghost2->y }, WHITE);
            else if (ghost2dead == 2)
                DrawTextureRec(ghostmovetexture[1], ghostmoveframe[1], { ghostdeadposx[1],ghostdeadposy[1] }, WHITE);
            if (ghost3dead == 0)
                DrawTextureRec(ghostmovetexture[2], ghostmoveframe[2], { ghost3->x, ghost3->y }, WHITE);
            else if (ghost3dead == 2)
                DrawTextureRec(ghostmovetexture[2], ghostmoveframe[2], { ghostdeadposx[2],ghostdeadposy[2] }, WHITE);
            if (ghost4dead == 0)
                DrawTextureRec(ghostmovetexture[3], ghostmoveframe[3], { ghost4->x, ghost4->y }, WHITE);
            else if (ghost4dead == 2)
                DrawTextureRec(ghostmovetexture[3], ghostmoveframe[3], { ghostdeadposx[3],ghostdeadposy[3] }, WHITE);
            if (ghost5dead == 0)
                DrawTextureRec(ghostmovetexture[4], ghostmoveframe[4], { ghost5->x, ghost5->y }, WHITE);
            else if (ghost5dead == 2)
                DrawTextureRec(ghostmovetexture[4], ghostmoveframe[4], { ghostdeadposx[4],ghostdeadposy[4] }, WHITE);
            if (ghost6dead == 0)
                DrawTextureRec(ghostmovetexture[5], ghostmoveframe[5], { ghost6->x, ghost6->y }, WHITE);
            else if (ghost6dead == 2)
                DrawTextureRec(ghostmovetexture[5], ghostmoveframe[5], { ghostdeadposx[5],ghostdeadposy[5] }, WHITE);
            if (ghost7dead == 0)
                DrawTextureRec(ghostmovetexture[6], ghostmoveframe[6], { ghost7->x, ghost7->y }, WHITE);
            else if (ghost7dead == 2)
                DrawTextureRec(ghostmovetexture[6], ghostmoveframe[6], { ghostdeadposx[6],ghostdeadposy[6] }, WHITE);
            if (ghost8dead == 0)
                DrawTextureRec(ghostmovetexture[7], ghostmoveframe[7], { ghost8->x, ghost8->y }, WHITE);
            else if (ghost8dead == 2)
                DrawTextureRec(ghostmovetexture[7], ghostmoveframe[7], { ghostdeadposx[7],ghostdeadposy[7] }, WHITE);
            if (ghost9dead == 0)
                DrawTextureRec(ghostmovetexture[8], ghostmoveframe[8], { ghost9->x, ghost9->y }, WHITE);
            else if (ghost9dead == 2)
                DrawTextureRec(ghostmovetexture[8], ghostmoveframe[8], { ghostdeadposx[8],ghostdeadposy[8] }, WHITE);
            if (ghost10dead == 0)
                DrawTextureRec(ghostmovetexture[9], ghostmoveframe[9], { ghost10->x, ghost10->y }, WHITE);
            else if (ghost10dead == 2)
                DrawTextureRec(ghostmovetexture[9], ghostmoveframe[9], { ghostdeadposx[9],ghostdeadposy[9] }, WHITE);
        }
        DrawTextureRec(lightMask.texture, { 0, 0, (float)1000, -(float)1000 }, Vector2Zero(), ColorAlpha(WHITE, showLines ? 0.75f : 1.0f));
        if (ghost1dead == 0)
            if ((firstpeople->x - ghost1->x >= -50 && firstpeople->x - ghost1->x <= 50) && (firstpeople->y - ghost1->y >= -50 && firstpeople->y - ghost1->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[0] = ghost1->x;
                ghostdeadposy[0] = ghost1->y;
                ghostmovetexture[0] = texture3dead;
                ghostmoveframe[0] = framerec3dead;
                ghost1dead = 2;
                ghost6 = new Ghost(rand() % 1000, rand() % 1000);
                ghost6dead = 0;
                delete ghost1;
            }
        if (ghost1dead == 0)
            if ((secondpeople->x - ghost1->x >= -50 && secondpeople->x - ghost1->x <= 50) && (secondpeople->y - ghost1->y >= -50 && secondpeople->y - ghost1->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[0] = ghost1->x;
                ghostdeadposy[0] = ghost1->y;
                ghostmovetexture[0] = texture3dead;
                ghostmoveframe[0] = framerec3dead;
                ghost1dead = 2;
                ghost6 = new Ghost(rand() % 1000, rand() % 1000);
                ghost6dead = 0;
                delete ghost1;
            }
        if (ghost2dead == 0)
            if ((firstpeople->x - ghost2->x >= -50 && firstpeople->x - ghost2->x <= 50) && (firstpeople->y - ghost2->y >= -50 && firstpeople->y - ghost2->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[1] = ghost2->x;
                ghostdeadposy[1] = ghost2->y;
                ghostmovetexture[1] = texture3dead;
                ghostmoveframe[1] = framerec3dead;
                ghost2dead = 2;
                ghost7 = new Ghost(rand() % 1000, rand() % 1000);
                ghost7dead = 0;
                delete ghost2;
            }
        if (ghost2dead == 0)
            if ((secondpeople->x - ghost2->x >= -50 && secondpeople->x - ghost2->x <= 50) && (secondpeople->y - ghost2->y >= -50 && secondpeople->y - ghost2->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[1] = ghost2->x;
                ghostdeadposy[1] = ghost2->y;
                ghostmovetexture[1] = texture3dead;
                ghostmoveframe[1] = framerec3dead;
                ghost2dead = 2;
                ghost7 = new Ghost(rand() % 1000, rand() % 1000);
                ghost7dead = 0;
                delete ghost2;
            }
        if (ghost3dead == 0)
            if ((firstpeople->x - ghost3->x >= -50 && firstpeople->x - ghost3->x <= 50) && (firstpeople->y - ghost3->y >= -50 && firstpeople->y - ghost3->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[2] = ghost3->x;
                ghostdeadposy[2] = ghost3->y;
                ghostmovetexture[2] = texture3dead;
                ghostmoveframe[2] = framerec3dead;
                ghost3dead = 2;
                ghost8 = new Ghost(rand() % 1000, rand() % 1000);
                ghost8dead = 0;
                delete ghost3;
            }
        if (ghost3dead == 0)
            if ((secondpeople->x - ghost3->x >= -50 && secondpeople->x - ghost3->x <= 50) && (secondpeople->y - ghost3->y >= -50 && secondpeople->y - ghost3->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[2] = ghost3->x;
                ghostdeadposy[2] = ghost3->y;
                ghostmovetexture[2] = texture3dead;
                ghostmoveframe[2] = framerec3dead;
                ghost3dead = 2;
                ghost8 = new Ghost(rand() % 1000, rand() % 1000);
                ghost8dead = 0;
                delete ghost3;
            }
        if (ghost4dead == 0)
            if ((firstpeople->x - ghost4->x >= -50 && firstpeople->x - ghost4->x <= 50) && (firstpeople->y - ghost4->y >= -50 && firstpeople->y - ghost4->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[3] = ghost4->x;
                ghostdeadposy[3] = ghost4->y;
                ghostmovetexture[3] = texture3dead;
                ghostmoveframe[3] = framerec3dead;
                ghost4dead = 2;
                ghost9 = new Ghost(rand() % 1000, rand() % 1000);
                ghost9dead = 0;
                delete ghost4;
            }
        if (ghost4dead == 0)
            if ((secondpeople->x - ghost4->x >= -50 && secondpeople->x - ghost4->x <= 50) && (secondpeople->y - ghost4->y >= -50 && secondpeople->y - ghost4->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[3] = ghost4->x;
                ghostdeadposy[3] = ghost4->y;
                ghostmovetexture[3] = texture3dead;
                ghostmoveframe[3] = framerec3dead;
                ghost4dead = 2;
                ghost9 = new Ghost(rand() % 1000, rand() % 1000);
                ghost9dead = 0;
                delete ghost4;
            }
        if (ghost5dead == 0)
            if ((firstpeople->x - ghost5->x >= -50 && firstpeople->x - ghost5->x <= 50) && (firstpeople->y - ghost5->y >= -50 && firstpeople->y - ghost5->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[4] = ghost5->x;
                ghostdeadposy[4] = ghost5->y;
                ghostmovetexture[4] = texture3dead;
                ghostmoveframe[4] = framerec3dead;
                ghost5dead = 2;
                ghost10 = new Ghost(rand() % 1000, rand() % 1000);
                ghost10dead = 0;
                delete ghost5;
            }
        if (ghost5dead == 0)
            if ((secondpeople->x - ghost5->x >= -25 && secondpeople->x - ghost5->x <= 50) && (secondpeople->y - ghost5->y >= -50 && secondpeople->y - ghost5->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[4] = ghost5->x;
                ghostdeadposy[4] = ghost5->y;
                ghostmovetexture[4] = texture3dead;
                ghostmoveframe[4] = framerec3dead;
                ghost5dead = 2;
                ghost10 = new Ghost(rand() % 1000, rand() % 1000);
                ghost10dead = 0;
                delete ghost5;
            }
        if (ghost6dead == 0)
            if ((firstpeople->x - ghost6->x >= -25 && firstpeople->x - ghost6->x <= 50) && (firstpeople->y - ghost6->y >= -50 && firstpeople->y - ghost6->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[5] = ghost6->x;
                ghostdeadposy[5] = ghost6->y;
                ghostmovetexture[5] = texture3dead;
                ghostmoveframe[5] = framerec3dead;
                ghost6dead = 2;
                delete ghost6;
            }
        if (ghost6dead == 0)
            if ((secondpeople->x - ghost6->x >= -50 && secondpeople->x - ghost6->x <= 50) && (secondpeople->y - ghost6->y >= -50 && secondpeople->y - ghost6->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[5] = ghost6->x;
                ghostdeadposy[5] = ghost6->y;
                ghostmovetexture[5] = texture3dead;
                ghostmoveframe[5] = framerec3dead;
                ghost6dead = 2;
                delete ghost6;
            }
        if (ghost7dead == 0)
            if ((firstpeople->x - ghost7->x >= -50 && firstpeople->x - ghost7->x <= 50) && (firstpeople->y - ghost7->y >= -50 && firstpeople->y - ghost7->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[6] = ghost7->x;
                ghostdeadposy[6] = ghost7->y;
                ghostmovetexture[6] = texture3dead;
                ghostmoveframe[6] = framerec3dead;
                ghost7dead = 2;
                delete ghost7;
            }
        if (ghost7dead == 0)
            if ((secondpeople->x - ghost7->x >= -50 && secondpeople->x - ghost7->x <= 50) && (secondpeople->y - ghost7->y >= -50 && secondpeople->y - ghost7->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[6] = ghost7->x;
                ghostdeadposy[6] = ghost7->y;
                ghostmovetexture[6] = texture3dead;
                ghostmoveframe[6] = framerec3dead;
                ghost7dead = 2;
                delete ghost7;
            }
        if (ghost8dead == 0)
            if ((firstpeople->x - ghost8->x >= -50 && firstpeople->x - ghost8->x <= 50) && (firstpeople->y - ghost8->y >= -50 && firstpeople->y - ghost8->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[7] = ghost8->x;
                ghostdeadposy[7] = ghost8->y;
                ghostmovetexture[7] = texture3dead;
                ghostmoveframe[7] = framerec3dead;
                ghost8dead = 2;
                delete ghost8;
            }
        if (ghost8dead == 0)
            if ((secondpeople->x - ghost8->x >= -50 && secondpeople->x - ghost8->x <= 50) && (secondpeople->y - ghost8->y >= -50 && secondpeople->y - ghost8->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[7] = ghost8->x;
                ghostdeadposy[7] = ghost8->y;
                ghostmovetexture[7] = texture3dead;
                ghostmoveframe[7] = framerec3dead;
                ghost8dead = 2;
                delete ghost8;
            }
        if (ghost9dead == 0)
            if ((firstpeople->x - ghost9->x >= -50 && firstpeople->x - ghost9->x <= 50) && (firstpeople->y - ghost9->y >= -50 && firstpeople->y - ghost9->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[8] = ghost9->x;
                ghostdeadposy[8] = ghost9->y;
                ghostmovetexture[8] = texture3dead;
                ghostmoveframe[8] = framerec3dead;
                ghost9dead = 2;
                delete ghost9;
            }
        if (ghost9dead == 0)
            if ((secondpeople->x - ghost9->x >= -50 && secondpeople->x - ghost9->x <= 50) && (secondpeople->y - ghost9->y >= -50 && secondpeople->y - ghost9->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[8] = ghost9->x;
                ghostdeadposy[8] = ghost9->y;
                ghostmovetexture[8] = texture3dead;
                ghostmoveframe[8] = framerec3dead;
                ghost9dead = 2;
                delete ghost9;
            }
        if (ghost10dead == 0)
            if ((firstpeople->x - ghost10->x >= -50 && firstpeople->x - ghost10->x <= 50) && (firstpeople->y - ghost10->y >= -50 && firstpeople->y - ghost10->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[9] = ghost10->x;
                ghostdeadposy[9] = ghost10->y;
                ghostmovetexture[9] = texture3dead;
                ghostmoveframe[9] = framerec3dead;
                ghost10dead = 2;
                delete ghost10;
            }
        if (ghost10dead == 0)
            if ((secondpeople->x - ghost10->x >= -50 && secondpeople->x - ghost10->x <= 50) && (secondpeople->y - ghost10->y >= -50 && secondpeople->y - ghost10->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[9] = ghost10->x;
                ghostdeadposy[9] = ghost10->y;
                ghostmovetexture[9] = texture3dead;
                ghostmoveframe[9] = framerec3dead;
                ghost10dead = 2;
                delete ghost10;
            }

        if (secondpeople->point + firstpeople->point == 50) {
            if (secondpeople->point > firstpeople->point) {
                DrawText(TextFormat("Second player wins"), 1000, 300, 20, LIME);
                DrawText(TextFormat("                      Game Finished"), 1000, 400, 20, LIME);
                DrawTextureRec(textureend, framerecend, { 0, 0 }, WHITE);
            }
            else if (secondpeople->point < firstpeople->point) {
                DrawText(TextFormat("First player wins"), 1000, 300, 20, LIME);
                DrawText(TextFormat("                      Game Finished"), 1000, 400, 20, LIME);
                DrawTextureRec(textureend, framerecend, { 0, 0 }, WHITE);
            }
            else {
                DrawText(TextFormat("Draw"), 1000, 300, 20, LIME);
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
        EndDrawing();
    }

    UnloadRenderTexture(lightMask);
    CloseWindow(); 
    return 0;
}
