#pragma once
#include "MouseHandler.h"
#include "KeyHandler.h"
#include "GLHandler.h"
#include "UIAtlas.h"

class UIObject
{
protected:
	// Location settings 
	float loc_x,loc_y,width,height;
	GLfloat flatColor[4];

public:
	UIObject();
	virtual ~UIObject();
	
	// Getters and setters 
	void setLocation(float x, float y);
	float getX();
	void setX(float);
	float getY();
	void setY(float);
	void setSize(float w, float h);
	float getWidth();
	float getHeight();
	void setColor(float r, float g, float b);
	GLfloat* getColor();

	// Update the button 
	virtual void update(float deltaTime);
	// Update input 
	virtual void updateInput(KeyHandler* mKeyH, MouseHandler* mMouseH);
	// Draw the object to the screen
	// UIAtles must be bound first.
	virtual void draw(GLHandler* mgl, UIAtlas* mAtlas);

	// Check if the sent location is inside the objc
	virtual bool contains(float x, float y);
};

