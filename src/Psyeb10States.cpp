#pragma once
#include "Psyeb10States.h"

void Psyeb10States::enter()
{
}

void Psyeb10States::foreGroundStrings()
{
}

void Psyeb10States::initObjects()
{
}

void Psyeb10States::mainLoopPreUpdate()
{
}

void Psyeb10States::mouseDown(int iButton, int iX, int iY)
{
}

void Psyeb10States::reset()
{
}

void Psyeb10States::reEntry()
{
}

Psyeb10TileManager* Psyeb10States::getTileManager() {
	return &tm;
}


void Psyeb10States::copyAllBackgroundBuffer()
{
	engine->getForegroundSurface()->copyEntireSurface(engine->getBackgroundSurface());
}

void Psyeb10States::keyPressed(int iKeyCode)
{
	// Handle key press events here in override
}

void Psyeb10States::keyUp(int iKeyCode) {
	// Handle event when key up
}

void Psyeb10States::recieveUpdate(int code) {
	//Empty implementation
}