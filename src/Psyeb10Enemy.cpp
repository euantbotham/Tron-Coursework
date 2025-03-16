#include "header.h"
#include "Psyeb10Enemy.h"
#include "Psyeb10Engine.h"
Psyeb10Enemy::Psyeb10Enemy(BaseEngine* pEngine) : DisplayableObject(500,150,pEngine, 20,20,true)
{
	speedX = 0;
	speedY = 1;
	lastTileX = -1;
	lastTileY = -1;
	engine = dynamic_cast<Psyeb10Engine*>(pEngine);
	isPaused = false;
}

void Psyeb10Enemy::virtDoUpdate(int iCurrentTime)
{
	if (isPaused)
	{
		return;
	}
	
	
	if (engine->tm.isValidTilePosition(getXCentre(), getYCentre()))
	{
		int mapX = engine->tm.getMapXForScreenX(getXCentre()); // Which column?
		int mapY = engine->tm.getMapYForScreenY(getYCentre()); // Which row?
		int value = engine->tm.getMapValue(mapX, mapY); // Current value?
		// If square has not been painted on and landed on a new tile
		if (value == 0 && !(mapX == lastTileX && mapY == lastTileY))
		{
			engine->tm.setAndRedrawMapValueAt(mapX, mapY, 2, engine, engine->getBackgroundSurface());
			lastTileX = mapX;
			lastTileY = mapY;
			// If next move is invalid
			if (!(isValidMove(getXCentre() + 15*speedX, getYCentre()+15*speedY))) {
				// If moving up or down
				if (speedX == 0) {
					if (isValidMove(getXCentre() + 15, getYCentre())) {
						speedX = 1;
						speedY = 0;
					}
					else if (isValidMove(getXCentre() - 15, getYCentre())) {
						speedX = -1;
						speedY = 0;
					}
				}// If moving left to right
				else {
					if (isValidMove(getXCentre(), getYCentre()+ 15)) {
						speedX = 0;
						speedY = 1;
					}
					else if (isValidMove(getXCentre(), getYCentre()-15)) {
						speedX = 0;
						speedY = -1;
					}
				}
			}
		}
		else if (value != 0 && !(mapX == lastTileX && mapY == lastTileY)) {
			//TODO add death logic for enemy, Eventually make class so they both a sub of it 
			// Make a new function for subclasses and get this to just call them, make it virt.
			//Add logic to increase score
			engine->resetGame();
			speedX = 0;
			speedY = 1;
		}
	}
	m_iCurrentScreenX += speedX;
	m_iCurrentScreenY += speedY;
	// Ensure that the objects get redrawn on the display
	this->redrawDisplay();
}

void Psyeb10Enemy::virtDraw()
{
	getEngine()->drawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xFFD700);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + 3,
		m_iCurrentScreenY + 3,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX + m_iDrawWidth - 4, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + 3,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight - 4,
		m_iCurrentScreenX + 3,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX + m_iDrawWidth - 4, m_iCurrentScreenY + m_iDrawHeight - 4,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x000000);
}


bool Psyeb10Enemy::isValidMove(int x, int y) {
	if (engine->tm.isValidTilePosition(x, y)) {
		int mapX, mapY, tileVal;
		mapX = engine->tm.getMapXForScreenX(x);
		mapY = engine->tm.getMapYForScreenY(y);
		tileVal = (engine->tm.getMapValue(mapX, mapY));
		if (tileVal == 0) {
			return true;
		}
	}
	return false;
}


void Psyeb10Enemy::setPaused(bool paused)
{
	isPaused = paused;
}