#pragma once
#include "UIScreen.h"
#include "PauseScreen.h"
#include "Level.h"
#include "LevelEditor.h"

class FreePlayScreen : public UIScreen
{
	// States 
	bool paused, gameover;
	int timer;
	Level* level;
	PauseScreen* pauseScreen;
	LevelEditor levelEditor;

public:
	FreePlayScreen();
	virtual ~FreePlayScreen();

	// Initialize screen
	virtual void init(float screen_width, float screen_height);

	// Load screen
	// Note: If textures need to be loaded, they must
	// be loaded in this method or they will not work. 
	virtual void load(TextureAtlas* mAtlas);

	// Update the state of the screen
	virtual void update(float deltaTime);

	// Update the terrain list
	void updateTerrain();

	// Update input to the screen 
	virtual void updateInput(KeyHandler*, MouseHandler*);

	// Draw the screen
	virtual void draw(GLHandler* mgl, TextureAtlas* mAtlas);
};
