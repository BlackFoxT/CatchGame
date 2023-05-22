#pragma once

class Ghost
{
public:
	Ghost(int z, int t);
	~Ghost();
	float x, y;
	float speed;
	Texture2D ghostmovetexture;
	Rectangle ghostmoveframe;
	void up(Texture2D texture, Rectangle frame);
	void down(Texture2D texture, Rectangle frame);
	void right(Texture2D texture, Rectangle frame);
	void left(Texture2D texture, Rectangle frame);


};

