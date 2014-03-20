#include "GameScreen.h"

using namespace std;

GameScreen::GameScreen(SaveData* sd) : UIScreen()
{
	pauseScreen = NULL;
	level = NULL;
	paused = false;
	gameover = false;
	savedata = sd;
}

GameScreen::~GameScreen()
{
	delete level;
	delete pauseScreen;
}

// Initialize screen
void GameScreen::init(float screen_width, float screen_height){
	UIScreen::init(screen_width, screen_height);

	screenWidth = screen_width;
	screenHeight = screen_height;

	level = new Level();
	level->init(screen_width, screen_height, savedata);

	pauseScreen = new PauseScreen();
	pauseScreen->init(screen_width, screen_height);

	levelEditor.init(screen_width, screen_height);
}

// Load screen
// Note: If textures need to be loaded, they must
// be loaded in this method or they will not work. 
void GameScreen::load(TextureAtlas* mAtlas){
	UIScreen::load(mAtlas);

	levelEditor.load(mAtlas);
	levelEditor.setHandlers(&level->handlers);
}


// Unload screen textures
// THIS MUST BE CALLED IF YOU LOAD STUFF.
void GameScreen::unload(){
	UIScreen::unload();

	level->unload();
}

// Update the state of the screen
void GameScreen::update(float deltaTime){
	UIScreen::update(deltaTime);
	
	pauseScreen->update(deltaTime);

	if (!paused){
		levelEditor.update(deltaTime, &(level->handlers));
		if (!levelEditor.Enabled())
			level->update(deltaTime);
	}
}

// Update input to the screen 
void GameScreen::updateInput(KeyHandler* mKeyH, MouseHandler* mMouseH){
	UIScreen::updateInput(mKeyH, mMouseH);

	if (paused){
		pauseScreen->updateInput(mKeyH, mMouseH);

		// Check for quit
		if (pauseScreen->getTransitionCode() == SCREEN_MAIN){
			pauseScreen->setTransitionValue(NO_TRANSITION);
			transitionCode = SCREEN_MAIN_SAVE_GAME;
		}

		// Check for pause/unpause
		if (pauseScreen->getTransitionCode() == CLOSE_SCREEN ||
			mKeyH->keyReleased(KEY_P) || mKeyH->keyReleased(KEY_ESCAPE)){
			paused = false;
			pauseScreen->hide();
		}
	}
	else{
		// Update level editor input 
		levelEditor.updateInput(mKeyH, mMouseH, &(level->handlers));

		// Update level input if editor off
		if (!levelEditor.Enabled()){
			// Check for pause/unpause
			if (mKeyH->keyReleased(KEY_P) || mKeyH->keyReleased(KEY_ESCAPE)){
				paused = true;
				pauseScreen->show();
			}

			level->updateInput(mKeyH, mMouseH);
		}
	}
}

// Draw the screen
void GameScreen::draw(GLHandler* mgl, TextureAtlas* mAtlas){
	UIScreen::draw(mgl, mAtlas);

	mgl->setProjectionMatrix(mgl->orthoMatrix);

	level->draw(mgl, mAtlas);
	
	mAtlas->bindBuffers(mgl);
	mAtlas->bindTexture(mgl);

	level->drawUI(mgl, (UIAtlas*)mAtlas);
	levelEditor.draw(mgl, (UIAtlas*)mAtlas);
	pauseScreen->draw(mgl, mAtlas);
}


// Parse a command give
bool GameScreen::parseCommand(UITerminal* terminal, string command, string args){
	if (UIScreen::parseCommand(terminal, command, args))
		return true;

	// Update level editor commands if enabled 
	if (levelEditor.parseCommand(terminal, command, args))
		return true;

	// Check for reset commmand
	if (command == "level" && args == "reset"){
		delete level;

		level = new Level();
		level->init(screenWidth, screenHeight, savedata);
		// TODO this should load the level file in the future 

		terminal->addLine("level reset", TL_SUCCESS);

		return true;
	}
	// Set the light for the level
	else if (command == "setlight"){
		float value = (float)toDouble(args);

		if (value >= 0 && value <= 1){
			((LevelProperties*)level->handlers.levelProps)->setLight(value, value, value);
			terminal->addLine(command + " " + args, TL_SUCCESS);
		}
		else {
			terminal->addLine(args + " is not a valid light level", TL_WARNING);
			terminal->addLine(command + " " + args, TL_WARNING);
		}

		return true;
	}
	// Check for zoom command
	else if (command == "zoom"){

		if (args == "default"){
			// Set zoom and return 
			((Camera2D*)level->handlers.camera)->setZoom(1.0f);
			terminal->addLine(command + " " + args, TL_SUCCESS);
			return true;
		}

		// Grab zoom 
		double zoom = toDouble(args);

		// Dont allow negative zoom 
		if (zoom < 0.0)
		{
			terminal->addLine(command + args, TL_WARNING);
			terminal->addLine("Zoom must be positive\n", TL_WARNING);
			return true;
		}

		// Set zoom and return 
		((Camera2D*)level->handlers.camera)->setZoom((float)zoom);
		terminal->addLine(command + " " + args, TL_SUCCESS);
		return true;
	}
	// Check for damage command 
	else if (command == "damage"){
		string subCommand("none");
		string subArgs("none");

		UITerminal::getCommandAndArgs(&args, &subCommand, &subArgs);

		// Check for damage player 
		if (subCommand == "player"){
			double damage = toDouble(subArgs);
			((Player*)(level->handlers.player))->applyDamage((float)damage);
			terminal->addLine(command + " " + args, TL_SUCCESS);
			return true;
		}
		// Check for damage ufo 
		else if (subCommand == "ufo"){
			double damage = toDouble(subArgs);
			((Player*)(level->handlers.player))->ufo->applyDamage((float)damage);
			terminal->addLine(command + " " + args, TL_SUCCESS);
			return true;
		}

		terminal->addLine(command + " " + args, TL_WARNING);
		terminal->addLine("Unrecognized commands given to command: damage", TL_WARNING);
		return true;
	}

	return false;
}

