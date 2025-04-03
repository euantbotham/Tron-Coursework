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
	if (engine->tm.isValidTilePosition(getXCentre(), getYCentre()))
	{
		int mapX = engine->tm.getMapXForScreenX(getXCentre()); // Which column?
		int mapY = engine->tm.getMapYForScreenY(getYCentre()); // Which row?
		int value = engine->tm.getMapValue(mapX, mapY); // Current value?
		// If square has not been painted on
		if (value == 0 && !(mapX == lastTileX && mapY == lastTileY))
		{
			// Update this so it can be fixed later
			engine->tm.setAndRedrawMapValueAt(mapX, mapY, this->bikeValue, engine, engine->getBackgroundSurface());
			lastTileX = mapX;
			lastTileY = mapY;
		}
		else if (value != 0 && !(mapX == lastTileX && mapY == lastTileY)) {
			virtHandleDeath();
		}
	}

	m_iCurrentScreenX += speedX;
	m_iCurrentScreenY += speedY;

	virtPostMoveLogic();
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
