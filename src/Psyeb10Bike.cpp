#include "header.h"
#include "Psyeb10Bike.h"
#include "Psyeb10Engine.h"
Psyeb10Bike::Psyeb10Bike(int xStart, int yStart,
	BaseEngine* pEngine, int iWidth, int iHeight, int bikeVal) : DisplayableObject(xStart, yStart, pEngine, iWidth, iHeight, true) {
	// Set last tiles to -1
	lastTileX = -1;
	lastTileY = -1;

	speedX = 0;
	speedY = 0;

	isPaused = false;

	//Cast so can access specific features
	engine = dynamic_cast<Psyeb10Engine*>(getEngine());
	
	this->bikeValue = bikeVal;
}


void Psyeb10Bike::virtDoUpdate(int iCurrentTime)
{

	if (isPaused)
	{
		return;
	}

	// TODO update code so no longer needs to be a friend class
	if (engine->getTileManager()->isValidTilePosition(getXCentre(), getYCentre()))
	{
		int mapX = engine->getTileManager()->getMapXForScreenX(getXCentre()); // Which column?
		int mapY = engine->getTileManager()->getMapYForScreenY(getYCentre()); // Which row?
		int value = engine->getTileManager()->getMapValue(mapX, mapY); // Current value?
		// If square has not been painted on
		if (value == 0 && !(mapX == lastTileX && mapY == lastTileY))
		{
			// Update this so it can be fixed later
			engine->getTileManager()->setAndRedrawMapValueAt(mapX, mapY, this->bikeValue, engine, engine->getBackgroundSurface());
			lastTileX = mapX;
			lastTileY = mapY;
			virtPostMoveLogic();
		}
		else if (value != 0 && !(mapX == lastTileX && mapY == lastTileY)) {
			virtHandleDeath();
		}
	}

	m_iCurrentScreenX += speedX;
	m_iCurrentScreenY += speedY;

	// Ensure that the objects get redrawn on the display
	this->redrawDisplay();

}

void Psyeb10Bike::virtDraw()
{
	//here
}

//Handle death function for each bike, meant to be overriden
void Psyeb10Bike::virtHandleDeath()
{
}

// Logic that can be ran after each move on the grid
void Psyeb10Bike::virtPostMoveLogic()
{
}

void Psyeb10Bike::setPaused(bool paused) {
	isPaused = paused;
}

void Psyeb10Bike::virtKeyDown(int iKeyCode)
{
}

int Psyeb10Bike::getSpeedX() {
	return speedX;
}

int Psyeb10Bike::getSpeedY() {
	return speedY;
}