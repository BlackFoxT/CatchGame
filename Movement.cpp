#pragma once

class Movement
{
public:

	void up(Texture2D texture, Rectangle frame);
	void down(Texture2D texture, Rectangle frame);
	void right(Texture2D texture, Rectangle frame);
	void left(Texture2D texture, Rectangle frame);
	void upright(Texture2D texture, Rectangle frame);
	void upleft(Texture2D texture, Rectangle frame);
	void downright(Texture2D texture, Rectangle frame);
	void downleft(Texture2D texture, Rectangle frame);
};
