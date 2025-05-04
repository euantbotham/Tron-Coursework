#pragma once
#include "header.h"
#include "Psyeb10Engine.h"
#include "MyObjectB.h"
#include "MainCharacter.h"
#include "Psyeb10TileManager.h"
#include <sstream>
#include <string>
#include "SimpleImage.h"
#include "ImageManager.h"
#include "Psyeb10States.h"
#include "gameState.h"
#include "pauseState.h"
#include "mainMenuState.h"
#include "gameSetupState.h"
#include <SDL.h>
Psyeb10Engine::Psyeb10Engine()
{
	// Could potentially make this a smart pointer
	currentState = new mainMenuState(this);
	secondState = nullptr;
}


void Psyeb10Engine::virtSetupBackgroundBuffer()
{
	currentState->enter();
}


void Psyeb10Engine::virtDrawStringsOnTop()
{
	currentState->foreGroundStrings();
}


int Psyeb10Engine::virtInitialise() {
	// Initialize SDL_image for PNG support
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		return -1; // Return an error code
	}
	std::cout << "here" << std::endl;
	// Load the PNG icon
	SDL_Surface* iconSurface = IMG_Load("disk1.png");
	if (iconSurface != nullptr) {
		// Set the window icon
		SDL_SetWindowIcon(SDL_GetWindowFromID(1), iconSurface);
		SDL_FreeSurface(iconSurface); // Free the surface after setting the icon
	}
	else {
		std::cerr << "Failed to load icon: " << IMG_GetError() << std::endl;
	}
	return BaseEngine::virtInitialise();
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

void Psyeb10Engine::virtMainLoopDoBeforeUpdate()
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
	
	if (secondState != nullptr)
		delete secondState;
	
}

void Psyeb10Engine::setState(Psyeb10States* state , bool keepOldState, bool initState) {
	
	if (keepOldState) {
		delete secondState;
		secondState = currentState;
	}
	else {
		delete currentState;
	}
	
	this->currentState = state;
	if (initState) {
		clearContents();
		drawableObjectsChanged();
		lockBackgroundForDrawing();
		this->currentState->initObjects();
		this->currentState->enter();
		unlockBackgroundForDrawing();
	}
	redrawDisplay();
}


void Psyeb10Engine::setState()
{
	// Remove objects from arraY
	clearContents();
	delete this->currentState;
	this->currentState = secondState;
	secondState = nullptr;
	lockBackgroundForDrawing();
	this->currentState->reEntry();
	unlockBackgroundForDrawing();
	redrawDisplay();
}


Psyeb10States* Psyeb10Engine::getState(int stateNum) {
	if (stateNum == 0) {
		return this->currentState;
	}
	else {
		if (!secondState) {
			return nullptr;
		}
		else {
			return this->secondState;
		}
	}
}


void Psyeb10Engine::copyAllBackgroundBuffer()
{
	currentState->copyAllBackgroundBuffer();
}

void Psyeb10Engine::virtKeyDown(int iKeyCode)
{
	currentState->keyPressed(iKeyCode);
}

void Psyeb10Engine::setBackgroundSurface(DrawingSurface* surface) {
	m_pBackgroundSurface = surface;
}

void Psyeb10Engine::virtKeyUp(int iKeyCode)
{
	currentState->keyUp(iKeyCode);
}

void Psyeb10Engine::notifyState(int code) {
	currentState->recieveUpdate(code);
}

void Psyeb10Engine::virtMouseWheel(int x, int y, int which, int timestamp) {
	currentState->mouseWheelScrolled(x, y, which, timestamp);
}
