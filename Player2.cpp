#include "raylib.h"
#include "Player2.hpp"

Player2::Player2() {
	x = 300;
	y = 400;
	speed = 3;
	Texture2D p2movetexture;
	Rectangle p2moveframe;
}
void Player2::up(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	y -= a + speed;
	if (y < 0)
		y = 0;
}
void Player2::down(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	y += a + speed;
	if (y > 950)
		y = 950;
}
void Player2::right(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	x += a + speed;
	if (x > 950)
		x = 950;
}
void Player2::left(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	x -= a + speed;
	if (x < 0)
		x = 0;
}
void Player2::upright(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	x += a + speed;
	y -= a + speed;
	if (x > 950)
		x = 950;
	if (y < 0)
		y = 0;
}
void Player2::upleft(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	x -= a + speed;
	y -= a + speed;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
}
void Player2::downright(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	x += a + speed;
	y += a + speed;
	if (x > 950)
		x = 950;
	if (y > 950)
		y = 950;
}
void Player2::downleft(Texture2D texture, Rectangle frame, int a) {
	p2movetexture = texture;
	p2moveframe = frame;
	x -= a + speed;
	y += a + speed;
	if (x < 0)
		x = 0;
	if (y > 950)
		y = 950;
}
