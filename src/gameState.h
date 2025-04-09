#pragma once
#include "header.h"
#include "Psyeb10States.h"
#include "Psyeb10Engine.h"
#include "Psyeb10TileManager.h"
#include "MainCharacter.h"
#include "Psyeb10Enemy.h"

class Psyeb10Engine;
class MainCharacter;
class Psyeb10Enemy;

class gameState : public Psyeb10States
{
public:
	gameState(Psyeb10Engine* engineParam)
	{
		this->engine = engineParam;
		gameScore = 0;
		lastTick = -1;
		mainChar = NULL;
		enemy = NULL;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	Psyeb10TileManager* getTileManager();

protected:
	const int gridstartX = 350;
	const int gridstartY = 100;
	const int gridEndX = 950;
	const int gridEndY = 700;
	int gameScore;
	int lastTick;
	Psyeb10TileManager tm;
	MainCharacter* mainChar;
	Psyeb10Enemy* enemy;
};


