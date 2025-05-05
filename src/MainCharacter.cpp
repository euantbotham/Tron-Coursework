#include "header.h"
#include "MainCharacter.h"
#include "Psyeb10Engine.h"
#include "SimpleImage.h"
#include "UtilCollisionDetection.h"
#include <vector>

MainCharacter::MainCharacter(BaseEngine* pEngine) : Psyeb10Bike(650, 630, pEngine, 20, 20, 1)
{
	speedX = 0;
	speedY = -1;
	lives = 3;
	for (int i = 1; i <= 6; ++i) {
		std::string imageName = "MainChar" + std::to_string(i) + ".png";
		SimpleImage image = ImageManager::loadImage(imageName, false);
		image.setTransparencyColour(0); // Set transparency color if needed
		animationImages.push_back(image); // Add the image to the vector
	}
	for (int i = 1; i <= 3; ++i) {
		std::string imageName = "mainCharDeath" + std::to_string(i) + ".png";
		SimpleImage image = ImageManager::loadImage(imageName, false);
		image.setTransparencyColour(0); // Set transparency color if needed
		deathAnimationFrames.push_back(image); // Add the image to the vector
	}
}








void MainCharacter::virtKeyDown(int iKeyCode)
{
	if (isDying) {
		return; // Ignore key presses if the character is dying
	}
	switch (iKeyCode) {
	case SDLK_w:
		if (getDirection() != 2)
			changeDirection(0);
		break;
	case SDLK_s:
		if (getDirection() != 0)
			changeDirection(2);
		break;
	case SDLK_d:
		if (getDirection() != 3)
			changeDirection(1);
		break;
	case SDLK_a:
		if (getDirection() != 1)
			changeDirection(3);
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
}

void MainCharacter::virtHandleDeath()
{
	if (!isDead) {
		isDead = true; // Set the dead flag to true
		isDying = true;
		deathCurrentFrame = 0;
		deathLastFrameTime = getEngine()->getModifiedTime();
		speedX = 0;
		speedY = 0;
	}
	//Death animation finished
	if (!isDying) {
		lives--;
		isDead = false;
		engine->resetGame();
	}
	
}
