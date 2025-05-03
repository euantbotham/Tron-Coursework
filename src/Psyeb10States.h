#pragma once
#include "header.h"
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
	virtual void keyPressed(int iKeyCode);
	Psyeb10TileManager* getTileManager();
	virtual void keyUp(int iKeyCode);
	//Method to recieve an integer from the engine, is virtual so implementation can be overidden
	virtual void recieveUpdate(int code);
	
	// Maybe rewrite this better later
	virtual void copyAllBackgroundBuffer();
	virtual void mouseWheelScrolled(int x, int y, int which, int timestamp);
protected:
	Psyeb10Engine* engine;
	Psyeb10TileManager tm;
};
