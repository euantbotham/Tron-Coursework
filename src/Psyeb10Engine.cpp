#include "header.h"
#include "Psyeb10Engine.h"
#include "MyObjectB.h"
#include "MainCharacter.h"
#include "Psyeb10TileManager.h"
#include <sstream>
#include <string>
#include "Psyeb10Enemy.h"


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
	this->drawBackgroundString(600, 50, "TRON", 0x0000FF, 0);

	//Setup tile manager
	for (int i = 0; i < 120; i++)
		for (int j = 0; j < 120; j++)
			tm.setMapValue(i, j, 0);
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(this, getBackgroundSurface());
}


void Psyeb10Engine::virtDrawStringsOnTop()
{
	MainCharacter* mC = dynamic_cast<MainCharacter*>(getDisplayableObject(0));
	std::stringstream p1Lives;
	p1Lives << "lives : " << mC->getLives();
	std::string livesMessage = p1Lives.str();
	this->drawForegroundString(200, 200, livesMessage.c_str(), 0x0000FF,0 );
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