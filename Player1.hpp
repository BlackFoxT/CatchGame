#pragma once
#include "Movement.hpp"

class Player1:public Movement	
{
public:
	Player1();
	int point = 0;
	float x, y;
	float speed;
	Texture2D p1movetexture;
	Rectangle p1moveframe;
	void up(Texture2D texture, Rectangle frame, int a = 0);
	void down(Texture2D texture, Rectangle frame, int a = 0);
	void right(Texture2D texture, Rectangle frame, int a = 0);
	void left(Texture2D texture, Rectangle frame, int a = 0);
	void upright(Texture2D texture, Rectangle frame, int a = 0);
	void upleft(Texture2D texture, Rectangle frame, int a = 0);
	void downright(Texture2D texture, Rectangle frame, int a = 0);
	void downleft(Texture2D texture, Rectangle frame, int a = 0);
};
