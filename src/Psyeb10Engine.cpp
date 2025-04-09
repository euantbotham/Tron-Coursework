#include "header.h"
#include "Psyeb10Engine.h"
#include "MyObjectB.h"
#include "MainCharacter.h"
#include "Psyeb10TileManager.h"
#include <sstream>
#include <string>
#include "Psyeb10Enemy.h"
#include "SimpleImage.h"
#include "ImageManager.h"

Psyeb10Engine::Psyeb10Engine()
{
	gameScore = 0;
	lastTick = -1;
}



void Psyeb10Engine::virtSetupBackgroundBuffer()
{
	//TODO add lines to make a grid
	// grey walls
	this->drawBackgroundRectangle(325, 75, 975, 725, 0x2C3E50);

	//Blue background
	this->drawBackgroundRectangle(350, 100, 950, 700, 0x000822);

	//Grid vert lines for background
	for (int i = 0; i < 14; i++)
	{
		this->drawBackgroundThickLine(gridstartX + 50 * i, gridstartY+1, gridstartX  + 50 * i, gridEndY-1, 0x000000, 2);
	}
	
	//Grid horizontal lines for background
	for (int i = 0; i < 14; i++)
	{
		this->drawBackgroundThickLine(gridstartX + 1, gridstartY + 50 *i, gridEndX -1, gridstartY + 50 * i, 0x000000, 2);
	}
	
	//background text
	this->drawBackgroundString(710, 25, "C++", 0x57E2F9, 0);


	// Background Image used as game logo
	SimpleImage image = ImageManager::loadImage("MainCharacter.png", true);
	image.renderImageWithMask(getBackgroundSurface(), 0, 0, 575, 20, image.getWidth(), image.getHeight());

	//Setup tile managerW
	for (int i = 0; i < 120; i++)
		for (int j = 0; j < 120; j++)
			tm.setMapValue(i, j, 0);
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(this, getBackgroundSurface());
}


void Psyeb10Engine::virtDrawStringsOnTop()
{
	MainCharacter* mC = dynamic_cast<MainCharacter*>(getDisplayableObject(0));
	std::stringstream p1Lives, p1Score;
	p1Lives << "lives : " << mC->getLives();
	std::string livesMessage = p1Lives.str();
	this->drawForegroundString(875, 20, livesMessage.c_str(), 0x57E2F9,0 );
	
	
	p1Score << "SCORE : " << gameScore;
	livesMessage = p1Score.str();

	this->drawForegroundString(300, 20, livesMessage.c_str(), 0x57E2F9, 0);
}


int Psyeb10Engine::virtInitialiseObjects()
{
	notifyObjectsAboutKeys(true);
	drawableObjectsChanged();
	destroyOldObjects(true);
	createObjectArray(1);
	storeObjectInArray(0, new MainCharacter(this));
	appendObjectToArray(new Psyeb10Enemy(this));
	setAllObjectsVisible(true);
	return 0;
}

//Function to reset the game state after each death
void Psyeb10Engine::resetGame()
{
	lockBackgroundForDrawing();
	virtSetupBackgroundBuffer();
	unlockBackgroundForDrawing();
	redrawDisplay();
	
	//Reset bikes to start positions
	DisplayableObject* bike1 = getDisplayableObject(0);
	bike1->setPosition(650, 400);

	DisplayableObject* bike2 = getDisplayableObject(1);
	bike2->setPosition(500, 150);
	//TODO add death reset for AI

}

void Psyeb10Engine::virtMouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_RIGHT)
	{
		if (this->isPaused()) {
			this->unpause();
			dynamic_cast<MainCharacter*>(getDisplayableObject(0))->setPaused(false);
			dynamic_cast<Psyeb10Enemy*>(getDisplayableObject(1))->setPaused(false);
		}
		else {
			this->pause();
			dynamic_cast<MainCharacter*>(getDisplayableObject(0))->setPaused(true);
			dynamic_cast<Psyeb10Enemy*>(getDisplayableObject(1))->setPaused(true);
		}
	}
}

void Psyeb10Engine::virtMainLoopPreUpdate()
{
	
	int time = this->getModifiedTime()/1000;
	if (time > lastTick)
	{
		lastTick = time;
		gameScore += 10;
	}
}

Psyeb10TileManager* Psyeb10Engine::getTileManager()
{
	return &tm;
}