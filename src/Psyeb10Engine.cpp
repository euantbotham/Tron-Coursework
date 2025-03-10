#include "header.h"
#include "Psyeb10Engine.h"
#include "MyObjectB.h"
#include "MainCharacter.h"

void Psyeb10Engine::virtSetupBackgroundBuffer()
{
	this->drawBackgroundRectangle(350, 100, 950, 700, 0x2C3E50);

	//background text
	this->drawBackgroundString(100, 100, "hello world", 0x0000FF, 0);

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
