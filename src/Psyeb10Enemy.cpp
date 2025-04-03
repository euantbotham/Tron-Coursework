#include "header.h"
#include "Psyeb10Enemy.h"
#include "Psyeb10Engine.h"
#include "DisplayableObject.h"
#include "Psyeb10TileManager.h"
#include <iostream>
Psyeb10Enemy::Psyeb10Enemy(BaseEngine* pEngine) : Psyeb10Bike(500,150,pEngine, 20,20, 2)
{
	speedX = 0;
	speedY = 1;
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



void Psyeb10Enemy::virtPostMoveLogic()
{
	// Ensure that the objects get redrawn on the display
	this->redrawDisplay();
	// If next move is invalid
	if (!(this->isValidMove(this->getXCentre() + 15 * speedX, this->getYCentre() + 15 * speedY))) {
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
			if (isValidMove(getXCentre(), getYCentre() + 15)) {
				speedX = 0;
				speedY = 1;
			}
			else if (isValidMove(getXCentre(), getYCentre() - 15)) {
				speedX = 0;
				speedY = -1;
			}
		}
	}
}

bool Psyeb10Enemy::isValidMove(int x, int y) {
	if (engine->getTileManager()->isValidTilePosition(x, y)) {
		//return true;
		int mapX, mapY, tileVal;
		mapX = engine->getTileManager()->getMapXForScreenX(x);
		mapY = engine->getTileManager()->getMapYForScreenY(y);
		tileVal = (engine->getTileManager()->getMapValue(mapX, mapY));
		if (tileVal == 0) {
			return true;
		}
	}
	return false;
}



void Psyeb10Enemy::virtHandleDeath() 
{
	//TODO add death logic for enemy, Eventually make class so they both a sub of it 
	// Make a new function for subclasses and get this to just call them, make it virt.
	//Add logic to increase score
	engine->resetGame();
	speedX = 0;
	speedY = 1;
}
