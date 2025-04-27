#include "header.h"
#include "MainCharacter.h"
#include "Psyeb10Engine.h"
#include "SimpleImage.h"
#include "UtilCollisionDetection.h"

MainCharacter::MainCharacter(BaseEngine* pEngine) : Psyeb10Bike(650, 400, pEngine, 20, 20, 1)
{
	speedX = 0;
	speedY = -1;
	lives = 3;
	image = ImageManager::loadImage("MainCharacter.png", true);
	image.setTransparencyColour(0);
}




void MainCharacter::virtDraw()
{
	//image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX - 30, m_iCurrentScreenY, image.getWidth(), image.getHeight());
	image.renderImageApplyingMapping(getEngine(), getEngine()->getForegroundSurface(), m_iCurrentScreenX - 30, m_iCurrentScreenY - 35, image.getWidth(), image.getHeight(), &rotation);
	//rotation.setTurn(rotation.None);
	/*
	getEngine()->drawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x0000ff);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + 3,
		m_iCurrentScreenY + 3,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX +m_iDrawWidth -4, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth -1,
		m_iCurrentScreenY + 3,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight -4,
		m_iCurrentScreenX + 3,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x000000);
	getEngine()->drawForegroundRectangle(m_iCurrentScreenX + m_iDrawWidth - 4, m_iCurrentScreenY + m_iDrawHeight - 4,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x000000);
		*/
}



void MainCharacter::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {
	//TODO Add change image here too.
	case SDLK_w:
		rotation << rotation.Up;
		speedY = -1;
		speedX = 0;
		break;
	case SDLK_s:
		rotation << rotation.Down;
		speedY = 1;
		speedX = 0;
		break;
	case SDLK_d:
		rotation << rotation.Right;
		speedY = 0;
		speedX = 1;
		break;
	case SDLK_a:
		rotation << rotation.Left;
		speedY = 0;
		speedX = -1;
		break;
	default:
		break;
	}
	
}


int MainCharacter::getLives()
{
	return this->lives;
}

void MainCharacter::setLives(int newLives)
{
	this->lives = newLives;
}

void MainCharacter::virtPostMoveLogic()
{
	//Check object collisions if so reset the game and don't decrement lives as is a crash
	DisplayableObject* enemy = engine->getDisplayableObject(1);
	// Both objects are 20 by 20 rectangles so + and - 10 used to find bounds 
	if (CollisionDetection::checkRectangles(getXCentre() - 10, getXCentre() + 10, getYCentre() - 10, getYCentre() + 10,
		enemy->getXCentre() - 10, enemy->getXCentre() + 10, enemy->getYCentre() - 10, enemy->getYCentre() + 10)) {
		engine->resetGame();
	}
}

void MainCharacter::virtHandleDeath()
{
	lives--;
	engine->resetGame();

	//TODO Change this as does not work when both die
	speedX = 0;
	speedY = -1;
}
