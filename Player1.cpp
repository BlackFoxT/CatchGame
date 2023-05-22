#include "raylib.h"
#include "Player1.hpp"

Player1::Player1() {
	x = 625;
	y = 500;
	speed = 3;
	Texture2D p1movetexture;
	Rectangle p1moveframe;
}
void Player1::up(Texture2D texture, Rectangle frame) {
	p1movetexture = texture;
	p1moveframe = frame;
	y -= speed;
	if (y < 0)
		y = 0;
}
void Player1::down(Texture2D texture, Rectangle frame) {
	p1movetexture = texture;
	p1moveframe = frame;
	y += speed;
	if (y > 950)
		y = 950;
}
void Player1::right(Texture2D texture, Rectangle frame) {
	p1movetexture = texture;
	p1moveframe = frame;
	x += speed;
	if (x > 950)
		x = 950;
}
void Player1::left(Texture2D texture, Rectangle frame) {
	p1movetexture = texture;
	p1moveframe = frame;
	x -= speed;
	if (x < 0)
		x = 0;
}
