#pragma once

class Player1
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


};
