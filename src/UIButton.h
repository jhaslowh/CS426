#pragma once
#include "UITransitionObject.h"

#define UIB_TEXT_SIZE 20.0f

class UIButton : public UITransitionObject
{
	float text_x, text_y;
	bool clicked, hovered, down;
	char* label;
	
	GLfloat textColor[4];

public:
	UIButton();
	UIButton(float x, float y, float w, float h, char* l);
	~UIButton();

	// Getters and Setters 
	void setLabel(char* l);
	char* getLabel();

	// Update button 
	virtual void update(float deltaTime);
	// Update button input 
	virtual void updateInput(KeyHandler* mKeyH, MouseHandler* mMouseH);
	// Draw the button to the screen
	// UIAtles must be bound first.
	virtual void draw(GLHandler* mgl, UIAtlas* mAtlas);

	// Check if the button was clicked 
	bool wasClicked();
	// Center label text 
	void centerText(TextRender* mTR);
};

