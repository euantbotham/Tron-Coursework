#include "header.h"
#include "Psyeb10Engine.h"
#include "MyObjectB.h"
#include "MainCharacter.h"

void Psyeb10Engine::virtSetupBackgroundBuffer()
{
	//TODO add lines to make a grid
	this->drawBackgroundRectangle(350, 100, 950, 700, 0x2C3E50);

	//background text
	this->drawBackgroundString(100, 100, "hello world", 0x0000FF, 0);

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 30; j++)
			tm.setMapValue(i, j, 1);
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(this, getBackgroundSurface());
}


void Psyeb10Engine::virtDrawStringsOnTop()
{
	this->drawForegroundString(200, 200, "Lives : ", 0x0000FF, 0);
}


int Psyeb10Engine::virtInitialiseObjects()
{
	notifyObjectsAboutKeys(true);
	drawableObjectsChanged();
	destroyOldObjects(true);
	createObjectArray(1);
	storeObjectInArray(0, new MainCharacter(this));
	setAllObjectsVisible(true);
	return 0;
}
