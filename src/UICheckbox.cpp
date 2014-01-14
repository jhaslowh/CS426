#include "UICheckbox.h"

UICheckbox::UICheckbox() : UITransitionObject() {
	label = "none";
	checked = false;
	hovered = false;

	textColor[0] = 0.2f;
	textColor[1] = 0.2f;
	textColor[2] = 0.2f;
	textColor[3] = 1.0f;

	setHideType(HT_VERTICAL);
	setHideLocByDistance(100.0f);
}
UICheckbox::UICheckbox(float x, float y, float w, float h, char* l) : UITransitionObject(){
	loc_x = x;
	loc_y = y;
	width = w;
	height = h;
	label = l;
	checked = false;
	hovered = false;

	text_x = w + 10;
	text_y = (h/2.0f) - 
		(UIC_TEXT_SIZE/2.0f)+ 
		((UIC_TEXT_SIZE/TR_FONT_SIZE) * TR_FONT_BOTTOM_SPACE);

	textColor[0] = 0.2f;
	textColor[1] = 0.2f;
	textColor[2] = 0.2f;
	textColor[3] = 1.0f;

	setHideType(HT_VERTICAL);
	setHideLocByDistance(100.0f);
}
UICheckbox::~UICheckbox(){}

// Getters and Setters 
void UICheckbox::setLabel(char* l){label = l;}
char* UICheckbox::getLabel(){return label;}

// Update Checkbox
void UICheckbox::update(float deltaTime){
	UITransitionObject::update(deltaTime);
	if (mFadeOut) textColor[3] = flatColor[3];
}

// Update button input 
void UICheckbox::updateInput(KeyHandler* mKeyH, MouseHandler* mMouseH){
	if (shown()){
		// Check if button is hovered
		if (contains(mMouseH->getX(), mMouseH->getY()))
			hovered = true;
		else 
			hovered = false;

		// Check if button is clicked
		if (hovered && !mMouseH->isLeftDown() && mMouseH->wasLeftDown())
			checked = !checked;
	}
}

// Draw the button to the screen
// UIAtles must be bound first.
void UICheckbox::draw(GLHandler* mgl, UIAtlas* mAtlas){
	if (flatColor[3] != 0.0f){
		mgl->setFlatColor(flatColor);
		if (checked)
			mAtlas->draw(mgl, UII_CHECKBOX_CHECKED, loc_x, loc_y);
		else 
			mAtlas->draw(mgl, UII_CHECKBOX_NORMAL, loc_x, loc_y);

		mgl->setFlatColor(textColor);
		mAtlas->mTextRender->drawText(*mgl, label, 
			loc_x + text_x, loc_y + text_y, 0, UIC_TEXT_SIZE);
	}
}

// Check if the button was clicked 
bool UICheckbox::Checked(){
	return checked;
}