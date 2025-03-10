#include "header.h"
#include "MainCharacter.h"
#include "BaseEngine.h"
#include <iostream>
#include "SimpleImage.h"
#include "Psyeb10Engine.h"
#define MAXSPEED = 2.0

MainCharacter::MainCharacter(BaseEngine* pEngine) : DisplayableObject(100, 200, pEngine, 20, 20, true)
{
	//std::cout << "here" << std::endl;
	posX = 100;
	posY = 200;
	car_direction = 1;
	speedX = 1;
	speedY = 0;
}


//TODO current direction issues need to account that
void MainCharacter::virtDoUpdate(int iCurrentTime)
{	
	Psyeb10Engine* engine = dynamic_cast<Psyeb10Engine*>(getEngine());
	//std::cout << iCurrentTime << std::endl;
	if (engine->tm.isValidTilePosition( getXCentre(),  getYCentre()))
	{
		int mapX = engine->tm.getMapXForScreenX(getXCentre()); // Which column?
		int mapY = engine->tm.getMapYForScreenY(getYCentre()); // Which row?
		int value = engine->tm.getMapValue(mapX, mapY); // Current value?
		// If square has not been painted on
		if (value == 0)
		{
			engine->tm.setAndRedrawMapValueAt(mapX, mapY, 1, engine, engine->getBackgroundSurface());
		}
	}
	
	
	posY += speedY;
	posX += speedX;

	m_iCurrentScreenX = (int)posX;
	m_iCurrentScreenY = (int) posY;
	// Ensure that the objects get redrawn on the display
	this->redrawDisplay();
}

void MainCharacter::virtDraw()
{
	getEngine()->drawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xffffff);
}



void MainCharacter::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {
	// Add change image here too.
	case SDLK_w:
		speedY = -1;
		speedX = 0;
		break;
	case SDLK_s:
		speedY = 1;
		speedX = 0;
		break;
	case SDLK_d:
		speedY = 0;
		speedX = 1;
		break;
	case SDLK_a:
		speedY = 0;
		speedX = -1;
		break;
	default:
		break;
	}
	
}