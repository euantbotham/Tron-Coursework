#pragma once
#include "header.h"
#include "pauseState.h"

void pauseState::enter()
{
	engine->fillBackground(0x00FF00);
	engine->drawBackgroundString(400, 400, "Game Paused", 0x000000, 0);
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
}


void pauseState::reset()
{
}