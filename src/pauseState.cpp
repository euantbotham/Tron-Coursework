#pragma once
#include "header.h"
#include "pauseState.h"
#include <iostream>
#include <fstream> 
#include "Psyeb10TileManager.h"


void pauseState::enter()
{
	//engine->fillBackground(0x00FF00);
	
	engine->drawBackgroundRectangle(475, 150, 825, 575, 0x000000);
	engine->drawBackgroundString(525, 200, "Game Paused", 0x57E2F9, 0);
}

void pauseState::foreGroundStrings()
{
	tm.setTopLeftPositionOnScreen(525, 250);
	tm.setMapValue(0, 0, 1);
	tm.setMapValue(0, 1, 2);
	tm.drawAllTiles(engine, engine->getForegroundSurface());
}

void pauseState::initObjects()
{
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
				saveGame();
				engine->setExitWithCode(0);
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

void pauseState::saveGame()
{
	Psyeb10TileManager* saveGameTM = engine->getState(0)->getTileManager();
	if (!saveGameTM) {
		std::cerr << "Error: TileManager is null or invalid!" << std::endl;
		return;
	}

	std::ofstream outFile("tile_data.csv");
	if (!outFile.is_open()) {
		std::cerr << "Error: Could not open file for writing!" << std::endl;
		return;
	}

	const int width = saveGameTM->getMapWidth();
	const int height = saveGameTM->getMapHeight();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			outFile << saveGameTM->getMapValue(x, y);
			if (x < width - 1) {
				outFile << ",";
			}
		}
		outFile << "\n";
	}
	outFile.close();
	std::cout << "Game saved to tile_data.csv!" << std::endl;
}