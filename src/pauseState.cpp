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
	
	engine->drawBackgroundRectangle(475, 150, 825, 575, 0x000000);
	engine->drawBackgroundString(525, 200, "Game Paused", 0x57E2F9, 0);
}

void pauseState::foreGroundStrings()
{
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

void pauseState::saveGame()
{
	Psyeb10TileManager* saveGameTM = engine->getState(1)->getTileManager();
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

	// Save game stats to game_stats.txt
	std::ofstream statsFile("game_stats.txt");
	if (!statsFile.is_open()) {
		std::cerr << "Error: Could not open game_stats.txt for writing!" << std::endl;
		return;
	}

	gameState* game = dynamic_cast<gameState*>(engine->getState(1));
	if (!game) {
		std::cerr << "Error: gameState is null or invalid!" << std::endl;
		return;
	}

	// Save main character stats
	MainCharacter* mainChar = game->getmainChar();
	if (mainChar) {
		statsFile << "# Main Character\n";
		statsFile << "Position: " << mainChar->getXCentre() << " " << mainChar->getYCentre() << "\n";
		statsFile << "Speed: " << mainChar->getSpeedX() << " " << mainChar->getSpeedY() << "\n";
		statsFile << "Lives: " << mainChar->getLives() << "\n";
		// Can't have spaces due to how the file is read
		statsFile << "LastTiles: " << mainChar->getLastTileX() << " " << mainChar->getLastTileY() << "\n";
	}

	// Save enemy stats
	Psyeb10Bike* enemy = game->getEnemy();
	if (enemy) {
		statsFile << "# Enemy\n";
		statsFile << "Position: " << enemy->getXCentre() <<  " " << enemy->getYCentre() << "\n";
		statsFile << "Speed: " << enemy->getSpeedX() << " " << enemy->getSpeedY() << "\n";
		// Can't have spaces due to how the file is read
		statsFile << "LastTiles: " << enemy->getLastTileX() << " " << enemy->getLastTileY() << "\n";
	}

	// Save game state stats
	statsFile << "# Game State\n";
	statsFile << "Score: " << game->getGameScore() << "\n";

	statsFile.close();
	std::cout << "Game stats saved to game_stats.txt!" << std::endl;
}