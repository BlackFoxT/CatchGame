#include "raylib.h"
#include "Ghost.hpp"


Ghost::Ghost(int z, int t) {
	x = z;
	y = t;
	speed = 2;
}
Ghost::~Ghost() {
}
void Ghost::up() {
	//up button 

	y -= speed;
	if (y < 0)
		y = 0;

}
void Ghost::down() {
	//down button
	y += speed;
	if (y > 950)
		y = 950;
}
void Ghost::right() {
	//right button
	x += speed;
	if (x > 950)
		x = 950;
}
void Ghost::left() {
	//left button
	x -= speed;
	if (x < 0)
		x = 0;

}
