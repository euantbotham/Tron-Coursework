#include "header.h"
#include "MainCharacter.h"
#include "Psyeb10Engine.h"
#include "SimpleImage.h"
#include "UtilCollisionDetection.h"
#include <vector>

MainCharacter::MainCharacter(BaseEngine* pEngine) : Psyeb10Bike(650, 400, pEngine, 20, 20, 1)
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
}








void MainCharacter::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {
	case SDLK_w:
		changeDirection(0);
		break;
	case SDLK_s:
		changeDirection(2);
		break;
	case SDLK_d:
		changeDirection(1);
		break;
	case SDLK_a:
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
	changeDirection(0);
}
