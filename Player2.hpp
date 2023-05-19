#pragma once

class Player2
{
public:
	Player2();
	int point = 0;
	float x, y;
	float speed;
	void up();
	void down();
	void right();
	void left();

};