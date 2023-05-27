#pragma once

class Player2
{
public:
	Player2();
	int point = 0;
	float x, y;
	float speed;
	Texture2D p2movetexture;
	Rectangle p2moveframe;
	void up(Texture2D texture, Rectangle frame);
	void down(Texture2D texture, Rectangle frame);
	void right(Texture2D texture, Rectangle frame);
	void left(Texture2D texture, Rectangle frame);
	void upright(Texture2D texture, Rectangle frame);
	void upleft(Texture2D texture, Rectangle frame);
	void downright(Texture2D texture, Rectangle frame);
	void downleft(Texture2D texture, Rectangle frame);

};
