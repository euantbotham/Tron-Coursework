#include "gameState.h"
#include "MainCharacter.h"
#include <sstream>
#include <string>
#include "Psyeb10Enemy.h"

void gameState::enter()
{
	//TODO add lines to make a grid
	// grey walls
	engine->drawBackgroundRectangle(325, 75, 975, 725, 0x2C3E50);

	//Blue background
	engine->drawBackgroundRectangle(350, 100, 950, 700, 0x000822);

	//Grid vert lines for background
	for (int i = 0; i < 14; i++)
	{
		engine->drawBackgroundThickLine(gridstartX + 50 * i, gridstartY + 1, gridstartX + 50 * i, gridEndY - 1, 0x000000, 2);
	}

	//Grid horizontal lines for background
	for (int i = 0; i < 14; i++)
	{
		engine->drawBackgroundThickLine(gridstartX + 1, gridstartY + 50 * i, gridEndX - 1, gridstartY + 50 * i, 0x000000, 2);
	}

	//background text
	engine->drawBackgroundString(710, 25, "C++", 0x57E2F9, 0);


	// Background Image used as game logo
	SimpleImage image = ImageManager::loadImage("MainCharacter.png", true);
	image.renderImageWithMask(engine->getBackgroundSurface(), 0, 0, 575, 20, image.getWidth(), image.getHeight());

	//Setup tile managerW
	for (int i = 0; i < 120; i++)
		for (int j = 0; j < 120; j++)
			tm.setMapValue(i, j, 0);
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(engine, engine->getBackgroundSurface());
}


void gameState::foreGroundStrings()
{
	std::stringstream p1Lives, p1Score;
	p1Lives << "lives : " << mainChar->getLives();
	std::string livesMessage = p1Lives.str();
	engine->drawForegroundString(875, 20, livesMessage.c_str(), 0x57E2F9, 0);


	p1Score << "SCORE : " << gameScore;
	livesMessage = p1Score.str();

	engine->drawForegroundString(300, 20, livesMessage.c_str(), 0x57E2F9, 0);
}

void gameState::initObjects()
{
	engine->notifyObjectsAboutKeys(true);
	engine->drawableObjectsChanged();
	engine->destroyOldObjects(true);
	engine->createObjectArray(1);
	this->mainChar = new MainCharacter(engine);
	engine->storeObjectInArray(0, mainChar);
	this->enemy = new Psyeb10Enemy(engine);
	engine->appendObjectToArray(enemy);
	engine->setAllObjectsVisible(true);
}

void gameState::reset()
{
	engine->lockBackgroundForDrawing();
	enter();
	engine->unlockBackgroundForDrawing();
	engine->redrawDisplay();

	//Reset bikes to start positions
	DisplayableObject* bike1 = engine->getDisplayableObject(0);
	bike1->setPosition(650, 400);

	DisplayableObject* bike2 = engine->getDisplayableObject(1);
	bike2->setPosition(500, 150);
	//TODO add death reset for AI
}

void gameState::mouseDown(int iButton, int iX, int iY) {
	if (iButton == SDL_BUTTON_RIGHT)
	{
		if (engine->isPaused()) {
			engine->unpause();
			this->mainChar->setPaused(false);
			this->enemy->setPaused(false);
		}
		else {
			engine->pause();
			this->mainChar->setPaused(true);
			this->enemy->setPaused(true);
			engine->setState();
		}
	}
}

void gameState::mainLoopPreUpdate() {
	int time = engine->getModifiedTime() / 1000;
	if (time > lastTick)
	{
		lastTick = time;
		gameScore += 10;
	}
}

Psyeb10TileManager* gameState::getTileManager() {
	return &tm;
}
