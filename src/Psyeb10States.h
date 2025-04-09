#pragma once
#include "header.h"
#include "Psyeb10Engine.h"
#include "Psyeb10TileManager.h"

class Psyeb10Engine;

class Psyeb10States
{
public:
	virtual void enter();
	virtual void reEntry();
	virtual void foreGroundStrings();
	virtual void initObjects();
	virtual void reset();
	virtual void mouseDown(int iButton, int iX, int iY);
	virtual void mainLoopPreUpdate();
	virtual ~Psyeb10States() {}
protected:
	Psyeb10Engine* engine;
};
