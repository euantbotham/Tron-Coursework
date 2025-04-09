#pragma once
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
#include "pauseState.h"

Psyeb10Engine::Psyeb10Engine()
{
	// Could potentially make this a smart pointer
	//game = new gameState(this);
	currentState = new gameState(this);
	//this->pause = new pauseState(this);
	
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
	//TODO Fix this later see which states need tile managers, if all then its good
	return dynamic_cast<gameState*>(currentState)->getTileManager();
}


void Psyeb10Engine::virtCleanUp() {
	delete currentState;
}

void Psyeb10Engine::setState() {
	this->currentState = new pauseState(this);
	lockBackgroundForDrawing();
	this->currentState->enter();
	unlockBackgroundForDrawing();
	redrawDisplay();
}