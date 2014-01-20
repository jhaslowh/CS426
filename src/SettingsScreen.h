#pragma once
#include "UIScreen.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UICheckbox.h"
#include "Settings.h"
#include "FileHelper.h"

class SettingsScreen : public UIScreen
{
	Settings* settings;

	UILabel* labelTitle;
	UICheckbox* checkboxFullscreen;
	UIButton* buttonBack;

public:
	SettingsScreen(Settings* s);
	~SettingsScreen();

	// Initialize screen
	virtual void init(float screen_width, float screen_height);

	// Load screen
	virtual void load(TextureAtlas* mAtlas);

	// Update the state of the screen
	virtual void update(float deltaTime);

	// Update input to the screen 
	virtual void updateInput(KeyHandler*, MouseHandler*);

	// Draw the screen
	virtual void draw(GLHandler* mgl, TextureAtlas* mAtlas);

	// Hide the entire screen.
	// Any UI elements will need to be put into this function,
    // if they should be hidden when the screen is hidden.
	virtual void hide();

	// Show the entire screen.
	// All UI elements in the hide screen method should have show 
	// calls here. 
	virtual void show();
};
