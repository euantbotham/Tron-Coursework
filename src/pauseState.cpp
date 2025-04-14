#pragma once
#include "header.h"
#include "pauseState.h"
#include <iostream>


void pauseState::enter()
{
	//engine->fillBackground(0x00FF00);
	engine->drawBackgroundString(400, 400, "Game Paused", 0x000000, 0);
	

}

void pauseState::foreGroundStrings()
{
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.setMapValue(0, 0, 1);
	tm.drawAllTiles(engine, engine->getForegroundSurface());
}

void pauseState::initObjects()
{
}


void pauseState::mainLoopPreUpdate()
{
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