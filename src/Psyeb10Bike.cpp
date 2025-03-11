#include "header.h"
#include "Psyeb10Bike.h"

Psyeb10Bike::Psyeb10Bike(int xStart, int yStart, 
	BaseEngine* pEngine, int iWidth, int iHeight, 
	bool useTopLeftFor00 = true) : DisplayableObject(xStart, yStart, pEngine, iWidth, iHeight, useTopLeftFor00)
{
	// Set last tiles to -1
	lastTileX = -1;
	lastTileY = -1;

	//Cast so can access specific features
	engine = dynamic_cast<Psyeb10Engine*>(getEngine());
}


void Psyeb10Bike::virtDoUpdate(int iCurrentTime)
{
	//std::cout << iCurrentTime << std::endl;
	if (engine->tm.isValidTilePosition(getXCentre(), getYCentre()))
	{
		int mapX = engine->tm.getMapXForScreenX(getXCentre()); // Which column?
		int mapY = engine->tm.getMapYForScreenY(getYCentre()); // Which row?
		int value = engine->tm.getMapValue(mapX, mapY); // Current value?
		// If square has not been painted on
		if (value == 0 && !(mapX == lastTileX && mapY == lastTileY))
		{
			engine->tm.setAndRedrawMapValueAt(mapX, mapY, 1, engine, engine->getBackgroundSurface());
			lastTileX = mapX;
			lastTileY = mapY;
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