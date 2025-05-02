#include "header.h"
#include "Psyeb10Bike.h"
#include "Psyeb10Engine.h"
#include "ImagePixelMappingRotate90.h"
Psyeb10Bike::Psyeb10Bike(int xStart, int yStart,
	BaseEngine* pEngine, int iWidth, int iHeight, int bikeVal) : DisplayableObject(xStart, yStart, pEngine, iWidth, iHeight, true) {
	// Set the start position
	this->startposX = xStart;
	this->startPosY = yStart;
	
	// Set last tiles to -1
	lastTileX = xStart;
	lastTileY = yStart;

	speedX = 0;
	speedY = 0;

	isPaused = false;

	//Cast so can access specific features
	engine = dynamic_cast<Psyeb10Engine*>(getEngine());
	
	this->bikeValue = bikeVal;

	currentFrame = 0;
	lastFrameTime = 0;
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
			//Extremley Important, without this game crashes
			return;
		}
	}

	m_iCurrentScreenX += speedX;
	m_iCurrentScreenY += speedY;

	// Ensure that the objects get redrawn on the display
	this->redrawDisplay();

}

void Psyeb10Bike::virtDraw()
{
	// cycle through the images in the vector
	if (engine->getModifiedTime() - lastFrameTime > 150)
	{
		currentFrame = (currentFrame + 1) % animationImages.size();
		lastFrameTime = engine->getModifiedTime();
	}
	// Draw the current frame of the animation
	animationImages[currentFrame].renderImageApplyingMapping(getEngine(), getEngine()->getForegroundSurface(), m_iCurrentScreenX - 20,
		m_iCurrentScreenY - 20, animationImages[currentFrame].getWidth(), animationImages[currentFrame].getHeight(), &rotation);
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

void Psyeb10Bike::setSpeed(int x, int y) {
	this->speedX = x;
	this->speedY = y;
}

int Psyeb10Bike::getLastTileX() const {
	return lastTileX;
}

int Psyeb10Bike::getLastTileY() const {
	return lastTileY;
}

void Psyeb10Bike::setLastTiles(int x, int y) {
	this->lastTileX = x;
	this->lastTileY = y;
}


// Helper to change direction based on a direction code
void Psyeb10Bike::changeDirection(int direction)
{
	switch (direction) {
	case 0: // Up
		rotation << rotation.Up;
		speedX = 0;
		speedY = -1;
		break;
	case 1: // Right
		rotation << rotation.Right;
		speedX = 1;
		speedY = 0;
		break;
	case 2: // Down
		rotation << rotation.Down;
		speedX = 0;
		speedY = 1;
		break;
	case 3: // Left
		rotation << rotation.Left;
		speedX = -1;
		speedY = 0;
		break;
	}
}

int Psyeb10Bike::getBikeValue() const {
	return bikeValue;
}

void Psyeb10Bike::resetPos() {
	m_iCurrentScreenX = startposX;
	m_iCurrentScreenY = startPosY;
	lastTileX = -1;
	lastTileY = -1;
}