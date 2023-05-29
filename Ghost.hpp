#pragma once
#include "Movement.hpp"

class Ghost : public Movement
{
public: 
	Ghost(int z, int t);
	~Ghost();
	float x, y;
	float speed;
	Texture2D ghostmovetexture;
	Rectangle ghostmoveframe;
	void up(Texture2D texture, Rectangle frame, int a = 0);
	void down(Texture2D texture, Rectangle frame, int a = 0);
	void right(Texture2D texture, Rectangle frame, int a = 0);
	void left(Texture2D texture, Rectangle frame, int a = 0);
	void upright(Texture2D texture, Rectangle frame, int a = 0);
	void upleft(Texture2D texture, Rectangle frame, int a = 0);
	void downright(Texture2D texture, Rectangle frame, int a = 0);
	void downleft(Texture2D texture, Rectangle frame, int a = 0);

};
