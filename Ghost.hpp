#pragma once

class Ghost
{
public:
	Ghost(int z, int t);
	~Ghost();
	float x, y;
	float speed;
	void up();
	void down();
	void right();
	void left();


};