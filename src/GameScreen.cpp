#include "GameScreen.h"

using namespace std;

GameScreen::GameScreen(SaveData* sd) : UIScreen()
{
	pauseScreen = NULL;
	level = NULL;
	paused = false;
	savedata = sd;

	// Gameover 
	gameover = false;
	gameoverTime = 2.0f;
	cGameoverTime = 0.0f;
	gameOverX = gameOverY = 0.0f;

	// Victory 
	victory = false;
	victoryTime = 2.0f;
	cVictoryTime = 0.0f;
	victoryX = victoryY = 0.0f;
}

/*GameScreen::GameScreen(SaveData* sd, string levelLoad) : UIScreen()
{
	pauseScreen = NULL;
	level = NULL;
	paused = false;
	gameover = false;
	savedata = sd;
	gameoverTime = 4.0f;
	cGameoverTime = 0.0f;
	levelToLoad = levelLoad;
}*/

GameScreen::~GameScreen()
{
	delete level;
	delete pauseScreen;
}

// Initialize screen
void GameScreen::init(float screen_width, float screen_height, void* sh){
	UIScreen::init(screen_width, screen_height, sh);

	screenWidth = screen_width;
	screenHeight = screen_height;

	level = new Level();
	level->init(screen_width, screen_height, savedata);

	pauseScreen = new PauseScreen();
	pauseScreen->init(screen_width, screen_height, sh);

	levelEditor.init(screen_width, screen_height);
}

// Load screen
// Note: If textures need to be loaded, they must
// be loaded in this method or they will not work. 
void GameScreen::load(TextureAtlas* mAtlas){
	UIScreen::load(mAtlas);

	levelEditor.load(mAtlas);
	levelEditor.setHandlers(&level->handlers);

	UIAtlas* mUI = (UIAtlas*)mAtlas;
	gameOverX = (screenWidth * .5f) - (mUI->mTextRender->measureString("Gameover", 72.0f)*.5f);
	gameOverY = (screenHeight * .5f) - (72.0f * .5f);

	victoryX = (screenWidth * .5f) - (mUI->mTextRender->measureString("Victory", 72.0f)*.5f);
	victoryY = (screenHeight * .5f) - (72.0f * .5f);
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
	
	// If not in gameover state or victory state, update the pause screen, 
	// level editor and level. 
	if (!gameover && !victory){
		pauseScreen->update(deltaTime);

		if (!paused){
			levelEditor.update(deltaTime, &(level->handlers));
			if (!levelEditor.Enabled()){
				level->update(deltaTime);
				// Check for victory 
				victory = level->getVictory();

				// If player dies, increment save data and set gameover to true 
				Player* player = (Player*)(level->handlers.player);
				if (!player->alive()){
					savedata->incrAnimalCount(player->getAnimalCount());
					savedata->incrHumanCount(player->getHumanCount());
					gameover = true;
				}
			}
		}
	}
	else {
		if (gameover){
			cGameoverTime += deltaTime;
			if (cGameoverTime > gameoverTime){
				transitionCode = SCREEN_GAME_NEW;
				hide();
			}
		}
		else if (victory){
			cVictoryTime += deltaTime;
			if (cVictoryTime > victoryTime){
				transitionCode = SCREEN_CREDITS;
				hide();
			}
		}
	}
}

// Update input to the screen 
void GameScreen::updateInput(KeyHandler* mKeyH, MouseHandler* mMouseH){
	UIScreen::updateInput(mKeyH, mMouseH);

	// Only update input if not in gameover state 
	if (!gameover && !victory){
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

	// Draw gameover overlay
	if (gameover){
		UIAtlas* mUI = (UIAtlas*)mAtlas;
		mgl->setFlatColor(COLOR_BLACK_50);
		mUI->drawScale2(mgl, UII_REC,0.0f,0.0f,screenWidth,screenHeight);
		mgl->setFlatColor(COLOR_UI_LABEL);
		mUI->mTextRender->drawText(*mgl, "Gameover", gameOverX, gameOverY, 0.0f, 72.0f);
	}
	// Draw victory overlay 
	else if (victory){
		UIAtlas* mUI = (UIAtlas*)mAtlas;
		mgl->setFlatColor(COLOR_BLACK_50);
		mUI->drawScale2(mgl, UII_REC,0.0f,0.0f,screenWidth,screenHeight);
		mgl->setFlatColor(COLOR_UI_LABEL);
		mUI->mTextRender->drawText(*mgl, "Victory", victoryX, victoryY, 0.0f, 72.0f);
	}
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

