#include "header.h"
#include "Psyeb10Engine.h"
#include "MyObjectB.h"
#include "MainCharacter.h"
#include "Psyeb10TileManager.h"
#include <sstream>
#include <string>
#include "Psyeb10Enemy.h"
#include "SimpleImage.h"
#include "ImageManager.h"
#include "Psyeb10States.h"
#include "gameState.h"

Psyeb10Engine::Psyeb10Engine()
{
	currentState = new gameState(this);
}



void Psyeb10Engine::virtSetupBackgroundBuffer()
{
	currentState->enter();
}


void Psyeb10Engine::virtDrawStringsOnTop()
{
	currentState->foreGroundStrings();
}


int Psyeb10Engine::virtInitialiseObjects()
{
	currentState->initObjects();
	return 0;
}

//Function to reset the game state after each death
void Psyeb10Engine::resetGame()
{
	currentState->reset();

}

void Psyeb10Engine::virtMouseDown(int iButton, int iX, int iY)
{
	currentState->mouseDown(iButton, iX, iY);
}

void Psyeb10Engine::virtMainLoopPreUpdate()
{
	currentState->mainLoopPreUpdate();
}

Psyeb10TileManager* Psyeb10Engine::getTileManager()
{
	return currentState->getTileManager();
}