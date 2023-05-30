#pragma once
#include "Movement.hpp"

class Player1 : public Movement
{
public:
	Player1();
	int point = 0;
	float x, y;
	float speed;
	Texture2D p1movetexture;
	Rectangle p1moveframe;
	void up(Texture2D texture, Rectangle frame);
	void down(Texture2D texture, Rectangle frame);
	void right(Texture2D texture, Rectangle frame);
	void left(Texture2D texture, Rectangle frame);
	void upright(Texture2D texture, Rectangle frame);
	void upleft(Texture2D texture, Rectangle frame);
	void downright(Texture2D texture, Rectangle frame);
	void downleft(Texture2D texture, Rectangle frame);
};
