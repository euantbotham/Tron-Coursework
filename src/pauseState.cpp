#pragma once
#include "header.h"
#include "pauseState.h"



void pauseState::enter()
{
	engine->fillBackground(0x00FF00);
	engine->drawBackgroundString(400, 400, "Game Paused", 0x000000, 0);
	tm.setTopLeftPositionOnScreen(350, 100);
	tm.drawAllTiles(engine, engine->getBackgroundSurface());
	

}

void pauseState::foreGroundStrings()
{
}

void pauseState::initObjects()
{
}

void pauseState::mainLoopPreUpdate()
{
}

void pauseState::mouseDown(int iButton, int iX, int iY)
{
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