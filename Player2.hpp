#pragma once
#include "Movement.hpp"

class Player2 : public Movement
{
public:
	Player2();
	int point = 0;
	float x, y;
	float speed;
	Texture2D p2movetexture;
	Rectangle p2moveframe;

	void up(Texture2D texture, Rectangle frame, int a = 0);
	void down(Texture2D texture, Rectangle frame, int a = 0);
	void right(Texture2D texture, Rectangle frame, int a = 0);
	void left(Texture2D texture, Rectangle frame, int a = 0);
	void upright(Texture2D texture, Rectangle frame, int a = 0);
	void upleft(Texture2D texture, Rectangle frame, int a = 0);
	void downright(Texture2D texture, Rectangle frame, int a = 0);
	void downleft(Texture2D texture, Rectangle frame, int a = 0);
};
