#include "raylib.h"
#include "Player1.hpp"

Player1::Player1() {
	x = 625;
	y = 500;
	speed = 3;
}
void Player1::up() {
	//up button 
	
	y -= speed;
	if (y < 0)
		y = 0;
}
void Player1::down() {
	//down button
	y += speed;
	if (y > 950)
		y = 950;
}
void Player1::right() {
	//right button
	x += speed;
	if (x > 950)
		x = 950;
}
void Player1::left() {
	//left button
	x -= speed;
	if (x < 0)
		x = 0;

}
