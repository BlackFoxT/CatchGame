#include "raylib.h"
#include "Ghost.hpp"

Ghost::Ghost(int z, int t) {
	x = z;
	y = t;
	speed = 2;
	Texture2D ghostmovetexture;
	Rectangle ghostmoveframe;
}
Ghost::~Ghost() {
}
void Ghost::up(Texture2D texture, Rectangle frame, int a) {
	//up button 
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	y -= a + speed;
	if (y < 0)
		y = 0;
}
void Ghost::down(Texture2D texture, Rectangle frame, int a) {
	//down button
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	y += a + speed;
	if (y > 950)
		y = 950;
}
void Ghost::right(Texture2D texture, Rectangle frame, int a) {
	//right button
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	x += a + speed;
	if (x > 950)
		x = 950;
}
void Ghost::left(Texture2D texture, Rectangle frame, int a) {
	//left button
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	x -= a + speed;
	if (x < 0)
		x = 0;
}
void Ghost::upright(Texture2D texture, Rectangle frame, int a) {
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	x += a + speed;
	y -= a + speed;
	if (x > 950)
		x = 950;
	if (y < 0)
		y = 0;
}
void Ghost::upleft(Texture2D texture, Rectangle frame, int a) {
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	x -= a + speed;
	y -= a + speed;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
}
void Ghost::downright(Texture2D texture, Rectangle frame, int a) {
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	x += a + speed;
	y += a + speed;
	if (x > 950)
		x = 950;
	if (y > 950)
		y = 950;
}
void Ghost::downleft(Texture2D texture, Rectangle frame, int a) {
	ghostmovetexture = texture;
	ghostmoveframe = frame;
	x -= a + speed;
	y += a + speed;
	if (x < 0)
		x = 0;
	if (y > 950)
		y = 950;
}
