#include "header.h"
#include "Psyeb10Enemy.h"
#include "Psyeb10Engine.h"
Psyeb10Enemy::Psyeb10Enemy(BaseEngine* pEngine) : DisplayableObject(0,0,pEngine, 20,20,true)
{
	speedX = 0;
	speedY = -1;
	lastTileX = -1;
	lastTileY = -1;
	engine = dynamic_cast<Psyeb10Engine*>(pEngine);
}

void Psyeb10Enemy::virtDoUpdate(int iCurrentTime)
{
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
			//TODO add death logic for enemy, Eventually make class so they both a sub of it 
			// Make a new function for subclasses and get this to just call them, make it virt.
		}
	}

	m_iCurrentScreenX += speedX;
	m_iCurrentScreenY += speedY;
	// Ensure that the objects get redrawn on the display
	this->redrawDisplay();
}

void Psyeb10Enemy::virtDraw()
{

}