#include "Button.h"

Button::Button(int Width, int Height, std::string spriteAddr, std::string Text, Vec2 Position) {

	sprite = LoadTextureFromImage(LoadImage(spriteAddr.c_str()));
	nPatchInfo = NPatchInfo(Rectangle(0, 0, sprite.width, sprite.height), 8, 8, 8, 8, NPATCH_NINE_PATCH);

	width = Width;
	height = Height;
	text = Text;
	position = Position;
	radius = height / 2;

	topLeft = Vec2(position.x, position.y);
	topRight = Vec2(position.x + width, position.y);
	bottomLeft = Vec2(position.x, position.y + height);
	bottomRight = Vec2(position.x + width, position.y + height);

	centerLeft = Vec2(position.x, position.y + height / 2);
	centerRight = Vec2(position.x + width, position.y + height / 2);

	justPressed = false;
	down = false;
	toggled = false;

}

bool Button::updateState(Vec2 mousePosition) {
	
	std::cout << mousePosition.x << " " << mousePosition.y << "\n";
	
	if (pointInRect(mousePosition, topRight, topLeft, bottomLeft, bottomRight) and IsMouseButtonDown(0))
		

	{
		std::cout << "Mouse down inside button\n";
		if (not down) {
			justPressed = true;
			toggled = not toggled;
		}
		else {
			justPressed = false;
		}

		
		down = true;
		return true;
	}
	
	down = false;
	return false;
	
}

void Button::draw() {
	DrawTextureNPatch(sprite, nPatchInfo, Rectangle(topLeft.x, topLeft.y, width, height), Vec2(0,0).toRaylib(), 0, WHITE);
	DrawRectangleLines(topLeft.x, topLeft.y, width, height, ORANGE);
	int textLength = MeasureText(text.c_str(), height - 8);
	DrawText(text.c_str(), topLeft.x + (width - textLength) / 2, topLeft.y + 4, height - 8, BLACK);
	
}
