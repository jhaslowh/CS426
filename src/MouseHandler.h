#pragma once
#include <SDL/SDL.h>
#include "Point.h"

class MouseHandler
{
	Point loc;
	bool last_left_down;
	bool last_right_down;
	bool left_down;
	bool right_down;

public:
	MouseHandler();
	~MouseHandler();

	// Get the mouse location 
	Point getLoc();
	// Get the x location 
	float getX();
	// Get the y location 
	float getY();
	// Check button states 
	bool isLeftDown();
	bool wasLeftDown();
	bool isRightDown();
	bool wasRightDown();

	// Call to update the mouse state
	void updateState(SDL_Event windowEvent);

	// Call at the end of the game update 
	void update();
};

