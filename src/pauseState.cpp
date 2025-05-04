#pragma once
#include "header.h"
#include "pauseState.h"
#include <iostream>
#include <fstream> 
#include "Psyeb10TileManager.h"
#include "gameState.h"
#include "Psyeb10Bike.h"
#include "MainCharacter.h"

void pauseState::enter()
{
	//engine->fillBackground(0x00FF00);
	
	//engine->drawBackgroundRectangle(475, 150, 825, 575, 0x000000);
	//engine->drawBackgroundString(525, 200, "Game Paused", 0x57E2F9, 0);
}

void pauseState::foreGroundStrings()
{
	//Draw the black rectangle
	engine->drawForegroundRectangle(475, 150, 825, 575, 0x000000);

	// Draw Tron blue lines around the rectangle
	engine->drawForegroundLine(475, 150, 825, 150, engine->tronBlue); // Top edge
	engine->drawForegroundLine(475, 575, 825, 575, engine->tronBlue); // Bottom edge
	engine->drawForegroundLine(475, 150, 475, 575, engine->tronBlue); // Left edge
	engine->drawForegroundLine(825, 150, 825, 575, engine->tronBlue); // Right edge


	engine->drawForegroundString(525, 200, "Game Paused", 0x57E2F9, 0);
	tm.setTopLeftPositionOnScreen(525, 250);
	//Resume game
	tm.setMapValue(0, 0, 1);
	// Save game and quit
	tm.setMapValue(0, 1, 2);
	// Quit game
	tm.setMapValue(0, 2, 4);
	tm.drawAllTiles(engine, engine->getForegroundSurface());
}

void pauseState::initObjects()
{
	logo = new bouncingLogo(100, 100, engine, 2, 2);
	engine->appendObjectToArray(logo);
}


void pauseState::mainLoopPreUpdate()
{
	int mx, my, tx, ty;
	mx = engine->getCurrentMouseX();
	my = engine->getCurrentMouseY();
	// If mouse hovers over tile notify tile manager
	if (tm.isValidTilePosition(mx, my)) {
		tx = tm.getMapXForScreenX(mx);
		ty = tm.getMapYForScreenY(my);
		tm.tileSelected(engine, engine->getForegroundSurface(), tx, ty);
		
	}
	else {
		// Notify if not over tile
		tm.tileSelected(engine, engine->getForegroundSurface(), -1, -1);
	}
}

void pauseState::mouseDown(int iButton, int iX, int iY)
{
	int x, y;
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (tm.isValidTilePosition(iX, iY))
		{
			x = tm.getMapXForScreenX(iX);
			y = tm.getMapYForScreenY(iY);
			if (tm.getMapValue(x, y) == 1)
			{
				engine->unpause();
				engine->setState();
			}
			else if (tm.getMapValue(x, y) == 2)
			{
				gameState* game = dynamic_cast<gameState*>(engine->getState(1));
				game->saveGame();
				engine->setExitWithCode(0);
			}
			else if (tm.getMapValue(x, y) == 4)
			{
				engine->setExitWithCode(0);
			}
			else
			{
				std::cout << "Not ready" << std::endl;
			}
		}
	}
	
	if (iButton == SDL_BUTTON_RIGHT)
	{
		engine->unpause();
		engine->setState();
	}
}


void pauseState::reset()
{
}

void pauseState::reEntry()
{
}

pauseState::~pauseState()
{
	delete this->logo;
}
