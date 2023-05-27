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
    Vector2 vertices[4];
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

void GhostMove(int* step, int* ghostdirection, Ghost* ghost, Texture2D texture3up, Rectangle framerec3up, Texture2D texture3down, Rectangle framerec3down, Texture2D texture3left, Rectangle framerec3left, Texture2D texture3right, Rectangle framerec3right, Texture2D texture3upright, Rectangle framerec3upright, Texture2D texture3upleft, Rectangle framerec3upleft, Texture2D texture3downright, Rectangle framerec3downright, Texture2D texture3downleft, Rectangle framerec3downleft) {
    if (*step < 0) {
        *ghostdirection = rand() % 8;
        *step = 150;
    }
    *step = *step-1;
    if (*ghostdirection == 0) {
        ghost->up(texture3up, framerec3up);
    }
    else if (*ghostdirection == 1) {
        ghost->down(texture3down, framerec3down);
    }
    else if (*ghostdirection == 2) {
        ghost->left(texture3left, framerec3left);
    }
    else if (*ghostdirection == 3) {
        ghost->right(texture3right, framerec3right);
    }
    else if (*ghostdirection == 4) {
        ghost->upright(texture3upright, framerec3upright);
    }
    else if (*ghostdirection == 5) {
        ghost->upleft(texture3upleft, framerec3upleft);
    }
    else if (*ghostdirection == 6) {
        ghost->downright(texture3downright, framerec3downright);
    }
    else if (*ghostdirection == 7) {
        ghost->downleft(texture3downleft, framerec3downleft);
    }
}

void MoveLight(int slot, float x, float y, LightInfo lights[MAX_LIGHTS]){
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

    lights[slot].shadows[lights[slot].shadowCount].vertices[0] = sp;
    lights[slot].shadows[lights[slot].shadowCount].vertices[1] = ep;
    lights[slot].shadows[lights[slot].shadowCount].vertices[2] = epProjection;
    lights[slot].shadows[lights[slot].shadowCount].vertices[3] = spProjection;
}

void DrawLightMask(int slot, LightInfo lights[MAX_LIGHTS]){
    BeginTextureMode(lights[slot].mask);
    ClearBackground(WHITE);
    rlSetBlendFactors(RLGL_SRC_ALPHA, RLGL_SRC_ALPHA, RLGL_MIN);
    rlSetBlendMode(BLEND_CUSTOM);

    if (lights[slot].valid) DrawCircleGradient((int)lights[slot].position.x + 25, (int)lights[slot].position.y + 25, lights[slot].outerRadius, ColorAlpha(WHITE, 0), WHITE);

    rlSetBlendMode(BLEND_ALPHA);
    rlSetBlendMode(BLEND_CUSTOM);
    rlSetBlendMode(BLEND_ALPHA);

    EndTextureMode();
}

void SetupLight(int slot, float x, float y, float radius, LightInfo lights[MAX_LIGHTS]){
    lights[slot].active = true;
    lights[slot].valid = false;
    lights[slot].mask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    lights[slot].outerRadius = radius;
    lights[slot].bounds.width = radius * 2;
    lights[slot].bounds.height = radius * 2;

    MoveLight(slot, x, y, lights);
    DrawLightMask(slot, lights);
}

bool UpdateLight(int slot, Rectangle* boxes, int count, LightInfo lights[MAX_LIGHTS]){
    if (!lights[slot].active || !lights[slot].dirty) return false;

    lights[slot].dirty = false;
    lights[slot].shadowCount = 0;
    lights[slot].valid = false;

    for (int i = 0; i < count; i++){
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

        lights[slot].shadows[lights[slot].shadowCount].vertices[0] = { boxes[i].x, boxes[i].y };
        lights[slot].shadows[lights[slot].shadowCount].vertices[1] = { boxes[i].x, boxes[i].y + boxes[i].height };
        lights[slot].shadows[lights[slot].shadowCount].vertices[2] = { boxes[i].x + boxes[i].width, boxes[i].y + boxes[i].height };
        lights[slot].shadows[lights[slot].shadowCount].vertices[3] = { boxes[i].x + boxes[i].width, boxes[i].y };
        lights[slot].shadowCount++;
    }
    lights[slot].valid = true;
    DrawLightMask(slot, lights);
    return true;
}

int main(void){
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

    Image pl1upleft = LoadImage("boy_upleft_1.png");
    Texture2D textureupleft = LoadTextureFromImage(pl1upleft);
    Rectangle framerecupleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1downleft = LoadImage("boy_downleft_1.png");
    Texture2D texturedownleft = LoadTextureFromImage(pl1downleft);
    Rectangle framerecdownleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1upright = LoadImage("boy_upright_1.png");
    Texture2D textureupright = LoadTextureFromImage(pl1upright);
    Rectangle framerecupright = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl1downright = LoadImage("boy_downright_1.png");
    Texture2D texturedownright = LoadTextureFromImage(pl1downright);
    Rectangle framerecdownright = { 0.10f,0.10f,(float)50,(float)50 };

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

    Image pl2upleft = LoadImage("boy_upleft_2.png");
    Texture2D texture2upleft = LoadTextureFromImage(pl2upleft);
    Rectangle framerec2upleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2downleft = LoadImage("boy_downleft_2.png");
    Texture2D texture2downleft = LoadTextureFromImage(pl2downleft);
    Rectangle framerec2downleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2upright = LoadImage("boy_upright_2.png");
    Texture2D texture2upright = LoadTextureFromImage(pl2upright);
    Rectangle framerec2upright = { 0.10f,0.10f,(float)50,(float)50 };

    Image pl2downright = LoadImage("boy_downright_2.png");
    Texture2D texture2downright = LoadTextureFromImage(pl2downright);
    Rectangle framerec2downright = { 0.10f,0.10f,(float)50,(float)50 };

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

    Image ghostupright = LoadImage("ghost_upright.png");
    Texture2D texture3upright = LoadTextureFromImage(ghostupright);
    Rectangle framerec3upright = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostupleft = LoadImage("ghost_upleft.png");
    Texture2D texture3upleft = LoadTextureFromImage(ghostupleft);
    Rectangle framerec3upleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostdownright = LoadImage("ghost_downright.png");
    Texture2D texture3downright = LoadTextureFromImage(ghostdownright);
    Rectangle framerec3downright = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostdownleft = LoadImage("ghost_downleft.png");
    Texture2D texture3downleft = LoadTextureFromImage(ghostdownleft);
    Rectangle framerec3downleft = { 0.10f,0.10f,(float)50,(float)50 };

    Image ghostdead = LoadImage("ghost_dead.png");
    Texture2D texture3dead = LoadTextureFromImage(ghostdead);
    Rectangle framerec3dead = { 0.10f,0.10f,(float)50,(float)50 };

    Ghost* ghost1 = new Ghost(rand() % 950, rand() % 950);
    Ghost* ghost2 = new Ghost(rand() % 950, rand() % 950);
    Ghost* ghost3 = new Ghost(rand() % 950, rand() % 950);
    Ghost* ghost4 = new Ghost(rand() % 950, rand() % 950);
    Ghost* ghost5 = new Ghost(rand() % 950, rand() % 950);
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

    int ghostdirections[] = { rand() % 8,rand() % 8,rand() % 8,rand() % 8,rand() % 8,rand() % 8,rand() % 8,rand() % 8,rand() % 8,rand() % 8 };
    int steps[] = { 150,150,150,150,150,150,150,150,150,150 };
    float ghostdeadposx[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
    float ghostdeadposy[] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };

    firstpeople->p1movetexture = texturestop;
    firstpeople->p1moveframe = framerecstop;
    secondpeople->p2movetexture = texture2stop;
    secondpeople->p2moveframe = framerec2stop;

    while (!WindowShouldClose()) {
        if (ghost1dead == 0)
            GhostMove(&steps[0], &ghostdirections[0], ghost1, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost2dead == 0)
            GhostMove(&steps[1], &ghostdirections[1], ghost2, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost3dead == 0)
            GhostMove(&steps[2], &ghostdirections[2], ghost3, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost4dead == 0)
            GhostMove(&steps[3], &ghostdirections[3], ghost4, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost5dead == 0)
            GhostMove(&steps[4], &ghostdirections[4], ghost5, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost6dead == 0)
            GhostMove(&steps[5], &ghostdirections[5], ghost6, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost7dead == 0)
            GhostMove(&steps[6], &ghostdirections[6], ghost7, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost8dead == 0)
            GhostMove(&steps[7], &ghostdirections[7], ghost8, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost9dead == 0)
            GhostMove(&steps[8], &ghostdirections[8], ghost9, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);
        if (ghost10dead == 0)
            GhostMove(&steps[9], &ghostdirections[9], ghost10, texture3up, framerec3up, texture3down, framerec3down, texture3left, framerec3left, texture3right, framerec3right, texture3upright, framerec3upright, texture3upleft, framerec3upleft, texture3downright, framerec3downright, texture3downleft, framerec3downleft);

        if (IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP)) {
            firstpeople->upright(textureupright, framerecupright);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_UP)) {
            firstpeople->upleft(textureupleft, framerecupleft);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_DOWN)) {
            firstpeople->downright(texturedownright, framerecdownright);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_DOWN)) {
            firstpeople->downleft(texturedownleft, framerecdownleft);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_RIGHT)) {
            firstpeople->right(textureright, framerecright);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_LEFT)) {
            firstpeople->left(textureleft, framerecleft);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_UP)) {
            firstpeople->up(textureup, framerecup);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_DOWN)) {
            firstpeople->down(texturedown, framerecdown);
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }
        else {
            firstpeople->p1movetexture = texturestop;
            firstpeople->p1moveframe = framerecstop;
            MoveLight(0, firstpeople->x, firstpeople->y, lights1);
            DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { firstpeople->x, firstpeople->y }, WHITE);
        }

        if (IsKeyDown(KEY_D) && IsKeyDown(KEY_W)) {
            secondpeople->upright(texture2upright, framerec2upright);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W)) {
            secondpeople->upleft(texture2upleft, framerec2upleft);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S)) {
            secondpeople->downright(texture2downright, framerec2downright);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_A) && IsKeyDown(KEY_S)) {
            secondpeople->downleft(texture2downleft, framerec2downleft);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_D)) {
            secondpeople->right(texture2right, framerec2right);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_A)) {
            secondpeople->left(texture2left, framerec2left);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_S)) {
            secondpeople->down(texture2down, framerec2down);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else if (IsKeyDown(KEY_W)) {
            secondpeople->up(texture2up, framerec2up);
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }
        else {
            secondpeople->p2movetexture = texture2stop;
            secondpeople->p2moveframe = framerec2stop;
            MoveLight(0, secondpeople->x, secondpeople->y, lights2);
            DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { secondpeople->x, secondpeople->y }, WHITE);
        }

        if (IsKeyPressed(KEY_F1)) showLines = !showLines;
        bool dirtyLights1 = false;
        bool dirtyLights2 = false;
        for (int i = 0; i < MAX_LIGHTS; i++) {
            if (UpdateLight(i, boxes, boxCount, lights1)) dirtyLights1 = true;
            if (UpdateLight(i, boxes, boxCount, lights2)) dirtyLights2 = true;
        }

        BeginTextureMode(lightMask);
        ClearBackground(BLACK);
        rlSetBlendMode(BLEND_CUSTOM);

        for (int i = 0; i < MAX_LIGHTS; i++) {
            if (lights1[i].active) DrawTextureRec(lights1[i].mask.texture, { 0, 0, (float)1000, -(float)1000 }, Vector2Zero(), WHITE);
            if (lights2[i].active) DrawTextureRec(lights2[i].mask.texture, { 0, 0, (float)1000, -(float)1000 }, Vector2Zero(), WHITE);
        }
        rlSetBlendMode(BLEND_ALPHA);
        EndTextureMode();
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextureRec(backgroundtexture, backgroundframerec, { 0,0 }, WHITE);
        DrawTextureRec(texture, framerec, position, WHITE);

        for (int i = 0; i < MAX_LIGHTS; i++) {
            if (lights1[i].active) DrawTextureRec(firstpeople->p1movetexture, firstpeople->p1moveframe, { (float)lights1[i].position.x, (float)lights1[i].position.y }, WHITE);
            if (lights2[i].active) DrawTextureRec(secondpeople->p2movetexture, secondpeople->p2moveframe, { (float)lights2[i].position.x, (float)lights2[i].position.y }, WHITE);
            if (ghost1dead == 0)
                DrawTextureRec(ghost1->ghostmovetexture, ghost1->ghostmoveframe, { ghost1->x, ghost1->y }, WHITE);
            else if (ghost1dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[0],ghostdeadposy[0] }, WHITE);
            if (ghost2dead == 0)
                DrawTextureRec(ghost2->ghostmovetexture, ghost2->ghostmoveframe, { ghost2->x, ghost2->y }, WHITE);
            else if (ghost2dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[1],ghostdeadposy[1] }, WHITE);
            if (ghost3dead == 0)
                DrawTextureRec(ghost3->ghostmovetexture, ghost3->ghostmoveframe, { ghost3->x, ghost3->y }, WHITE);
            else if (ghost3dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[2],ghostdeadposy[2] }, WHITE);
            if (ghost4dead == 0)
                DrawTextureRec(ghost4->ghostmovetexture, ghost4->ghostmoveframe, { ghost4->x, ghost4->y }, WHITE);
            else if (ghost4dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[3],ghostdeadposy[3] }, WHITE);
            if (ghost5dead == 0)
                DrawTextureRec(ghost5->ghostmovetexture, ghost5->ghostmoveframe, { ghost5->x, ghost5->y }, WHITE);
            else if (ghost5dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[4],ghostdeadposy[4] }, WHITE);
            if (ghost6dead == 0)
                DrawTextureRec(ghost6->ghostmovetexture, ghost6->ghostmoveframe, { ghost6->x, ghost6->y }, WHITE);
            else if (ghost6dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[5],ghostdeadposy[5] }, WHITE);
            if (ghost7dead == 0)
                DrawTextureRec(ghost7->ghostmovetexture, ghost7->ghostmoveframe, { ghost7->x, ghost7->y }, WHITE);
            else if (ghost7dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[6],ghostdeadposy[6] }, WHITE);
            if (ghost8dead == 0)
                DrawTextureRec(ghost8->ghostmovetexture, ghost8->ghostmoveframe, { ghost8->x, ghost8->y }, WHITE);
            else if (ghost8dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[7],ghostdeadposy[7] }, WHITE);
            if (ghost9dead == 0)
                DrawTextureRec(ghost9->ghostmovetexture, ghost9->ghostmoveframe, { ghost9->x, ghost9->y }, WHITE);
            else if (ghost9dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[8],ghostdeadposy[8] }, WHITE);
            if (ghost10dead == 0)
                DrawTextureRec(ghost10->ghostmovetexture, ghost10->ghostmoveframe, { ghost10->x, ghost10->y }, WHITE);
            else if (ghost10dead == 2)
                DrawTextureRec(texture3dead, framerec3dead, { ghostdeadposx[9],ghostdeadposy[9] }, WHITE);
        }
        DrawTextureRec(lightMask.texture, { 0, 0, (float)1000, -(float)1000 }, Vector2Zero(), ColorAlpha(WHITE, showLines ? 0.75f : 1.0f));
        if (ghost1dead == 0){
            if ((firstpeople->x - ghost1->x >= -50 && firstpeople->x - ghost1->x <= 50) && (firstpeople->y - ghost1->y >= -50 && firstpeople->y - ghost1->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[0] = ghost1->x;
                ghostdeadposy[0] = ghost1->y;
                ghost1dead = 2;
                ghost6 = new Ghost(rand() % 950, rand() % 950);
                ghost6dead = 0;
                delete ghost1;
            }
            else if ((secondpeople->x - ghost1->x >= -50 && secondpeople->x - ghost1->x <= 50) && (secondpeople->y - ghost1->y >= -50 && secondpeople->y - ghost1->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[0] = ghost1->x;
                ghostdeadposy[0] = ghost1->y;
                ghost1dead = 2;
                ghost6 = new Ghost(rand() % 950, rand() % 950);
                ghost6dead = 0;
                delete ghost1;
            }
        }
        if (ghost2dead == 0){
            if ((firstpeople->x - ghost2->x >= -50 && firstpeople->x - ghost2->x <= 50) && (firstpeople->y - ghost2->y >= -50 && firstpeople->y - ghost2->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[1] = ghost2->x;
                ghostdeadposy[1] = ghost2->y;
                ghost2dead = 2;
                ghost7 = new Ghost(rand() % 950, rand() % 950);
                ghost7dead = 0;
                delete ghost2;
            }
            else if ((secondpeople->x - ghost2->x >= -50 && secondpeople->x - ghost2->x <= 50) && (secondpeople->y - ghost2->y >= -50 && secondpeople->y - ghost2->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[1] = ghost2->x;
                ghostdeadposy[1] = ghost2->y;
                ghost2dead = 2;
                ghost7 = new Ghost(rand() % 950, rand() % 950);
                ghost7dead = 0;
                delete ghost2;
            }
        }
        if (ghost3dead == 0){
            if ((firstpeople->x - ghost3->x >= -50 && firstpeople->x - ghost3->x <= 50) && (firstpeople->y - ghost3->y >= -50 && firstpeople->y - ghost3->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[2] = ghost3->x;
                ghostdeadposy[2] = ghost3->y;
                ghost3dead = 2;
                ghost8 = new Ghost(rand() % 950, rand() % 950);
                ghost8dead = 0;
                delete ghost3;
            }
            else if ((secondpeople->x - ghost3->x >= -50 && secondpeople->x - ghost3->x <= 50) && (secondpeople->y - ghost3->y >= -50 && secondpeople->y - ghost3->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[2] = ghost3->x;
                ghostdeadposy[2] = ghost3->y;
                ghost3dead = 2;
                ghost8 = new Ghost(rand() % 950, rand() % 950);
                ghost8dead = 0;
                delete ghost3;
            }
        }
        if (ghost4dead == 0){
            if ((firstpeople->x - ghost4->x >= -50 && firstpeople->x - ghost4->x <= 50) && (firstpeople->y - ghost4->y >= -50 && firstpeople->y - ghost4->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[3] = ghost4->x;
                ghostdeadposy[3] = ghost4->y;
                ghost4dead = 2;
                ghost9 = new Ghost(rand() % 950, rand() % 950);
                ghost9dead = 0;
                delete ghost4;
            }
            else if ((secondpeople->x - ghost4->x >= -50 && secondpeople->x - ghost4->x <= 50) && (secondpeople->y - ghost4->y >= -50 && secondpeople->y - ghost4->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[3] = ghost4->x;
                ghostdeadposy[3] = ghost4->y;
                ghost4dead = 2;
                ghost9 = new Ghost(rand() % 950, rand() % 950);
                ghost9dead = 0;
                delete ghost4;
            }
        }
        if (ghost5dead == 0){
            if ((firstpeople->x - ghost5->x >= -50 && firstpeople->x - ghost5->x <= 50) && (firstpeople->y - ghost5->y >= -50 && firstpeople->y - ghost5->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[4] = ghost5->x;
                ghostdeadposy[4] = ghost5->y;
                ghost5dead = 2;
                ghost10 = new Ghost(rand() % 950, rand() % 950);
                ghost10dead = 0;
                delete ghost5;
            }
            else if ((secondpeople->x - ghost5->x >= -25 && secondpeople->x - ghost5->x <= 50) && (secondpeople->y - ghost5->y >= -50 && secondpeople->y - ghost5->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[4] = ghost5->x;
                ghostdeadposy[4] = ghost5->y;
                ghost5dead = 2;
                ghost10 = new Ghost(rand() % 950, rand() % 950);
                ghost10dead = 0;
                delete ghost5;
            }
        }
        if (ghost6dead == 0){
            if ((firstpeople->x - ghost6->x >= -25 && firstpeople->x - ghost6->x <= 50) && (firstpeople->y - ghost6->y >= -50 && firstpeople->y - ghost6->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[5] = ghost6->x;
                ghostdeadposy[5] = ghost6->y;
                ghost6dead = 2;
                delete ghost6;
            }
            else if ((secondpeople->x - ghost6->x >= -50 && secondpeople->x - ghost6->x <= 50) && (secondpeople->y - ghost6->y >= -50 && secondpeople->y - ghost6->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[5] = ghost6->x;
                ghostdeadposy[5] = ghost6->y;
                ghost6dead = 2;
                delete ghost6;
            }
         }
        if (ghost7dead == 0){
            if ((firstpeople->x - ghost7->x >= -50 && firstpeople->x - ghost7->x <= 50) && (firstpeople->y - ghost7->y >= -50 && firstpeople->y - ghost7->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[6] = ghost7->x;
                ghostdeadposy[6] = ghost7->y;
                ghost7dead = 2;
                delete ghost7;
            }
            else if ((secondpeople->x - ghost7->x >= -50 && secondpeople->x - ghost7->x <= 50) && (secondpeople->y - ghost7->y >= -50 && secondpeople->y - ghost7->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[6] = ghost7->x;
                ghostdeadposy[6] = ghost7->y;
                ghost7dead = 2;
                delete ghost7;
            }
        }
        if (ghost8dead == 0){
            if ((firstpeople->x - ghost8->x >= -50 && firstpeople->x - ghost8->x <= 50) && (firstpeople->y - ghost8->y >= -50 && firstpeople->y - ghost8->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[7] = ghost8->x;
                ghostdeadposy[7] = ghost8->y;
                ghost8dead = 2;
                delete ghost8;
            }
            else if ((secondpeople->x - ghost8->x >= -50 && secondpeople->x - ghost8->x <= 50) && (secondpeople->y - ghost8->y >= -50 && secondpeople->y - ghost8->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[7] = ghost8->x;
                ghostdeadposy[7] = ghost8->y;
                ghost8dead = 2;
                delete ghost8;
            }
        }
        if (ghost9dead == 0) {
            if ((firstpeople->x - ghost9->x >= -50 && firstpeople->x - ghost9->x <= 50) && (firstpeople->y - ghost9->y >= -50 && firstpeople->y - ghost9->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[8] = ghost9->x;
                ghostdeadposy[8] = ghost9->y;
                ghost9dead = 2;
                delete ghost9;
            }
            else if ((secondpeople->x - ghost9->x >= -50 && secondpeople->x - ghost9->x <= 50) && (secondpeople->y - ghost9->y >= -50 && secondpeople->y - ghost9->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[8] = ghost9->x;
                ghostdeadposy[8] = ghost9->y;
                ghost9dead = 2;
                delete ghost9;
            }
        }
        if (ghost10dead == 0){
            if ((firstpeople->x - ghost10->x >= -50 && firstpeople->x - ghost10->x <= 50) && (firstpeople->y - ghost10->y >= -50 && firstpeople->y - ghost10->y <= 50)) {
                firstpeople->point += 5;
                ghostdeadposx[9] = ghost10->x;
                ghostdeadposy[9] = ghost10->y;
                ghost10dead = 2;
                delete ghost10;
            }
            else if ((secondpeople->x - ghost10->x >= -50 && secondpeople->x - ghost10->x <= 50) && (secondpeople->y - ghost10->y >= -50 && secondpeople->y - ghost10->y <= 50)) {
                secondpeople->point += 5;
                ghostdeadposx[9] = ghost10->x;
                ghostdeadposy[9] = ghost10->y;
                ghost10dead = 2;
                delete ghost10;
            }
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
