#pragma once

#include "Vec2.h"
#include "fmTools.h"
#include <raylib.h>


class Button
{
public:
	Texture2D sprite;
	NPatchInfo nPatchInfo;
	

	int width;
	int height;
	int radius;
	std::string text;

	Vec2 dimensions;
	Vec2 position;

	Vec2 topLeft;
	Vec2 topRight;
	Vec2 bottomLeft;
	Vec2 bottomRight;
	Vec2 centerLeft;
	Vec2 centerRight;

	bool toggled;
	bool down;
	bool justPressed;

	Button(int width, int height, std::string spriteAddr, std::string text, Vec2 position);
	

	bool updateState(Vec2 mousePosition);
	void draw();
};

