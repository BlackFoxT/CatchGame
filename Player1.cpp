#include "raylib.h"
#include "Player1.hpp"

Player1::Player1() {
	x = 625;
	y = 500;
	speed = 3;
	Texture2D p1movetexture;
	Rectangle p1moveframe;
}/*
std::void void operator + (int t) {

}*/
void Player1::up(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	y -= a + speed;
	if (y < 0)
		y = 0;
}
void Player1::down(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	y += speed;
	if (y > 950)
		y = 950;
}
void Player1::right(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	x += a + speed;
	if (x > 950)
		x = 950;
}
void Player1::left(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	x -= a + speed;
	if (x < 0)
		x = 0;
}
void Player1::upright(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	x += a + speed;
	y -= a + speed;
	if (x > 950)
		x = 950;
	if (y < 0)
		y = 0;
}
void Player1::upleft(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	x -= a + speed;
	y -= a + speed;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
}
void Player1::downright(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	x += a + speed;
	y += a + speed;
	if (x > 950)
		x = 950;
	if (y > 950)
		y = 950;
}
void Player1::downleft(Texture2D texture, Rectangle frame, int a) {
	p1movetexture = texture;
	p1moveframe = frame;
	x -= a + speed;
	y += a + speed;
	if (x < 0)
		x = 0;
	if (y > 950)
		y = 950;
}
