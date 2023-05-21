#include "raylib.h"
#include "Player2.hpp"


Player2::Player2() {
	x = 300;
	y = 400;
	speed = 3;
}
void Player2::up() {
	//up button 

	y -= speed;
	if (y < 0)
		y = 0;
}
void Player2::down() {
	//down button
	y += speed;
	if (y > 950)
		y = 950;
}
void Player2::right() {
	//right button
	x += speed;
	if (x > 950)
		x = 950;
}
void Player2::left() {
	//left button
	x -= speed;
	if (x < 0)
		x = 0;

}
