#pragma once
#include "header.h"
#include "Psyeb10States.h"
#include "Psyeb10Engine.h"
#include "Psyeb10TileManager.h"
#include "MainCharacter.h"
#include "Psyeb10Enemy.h"

class Psyeb10Engine;

class gameState : public Psyeb10States
{
public:
	gameState(Psyeb10Engine* engineParam)
	{
		this->engine = engineParam;
		gameScore = 0;
		lastTick = -1;
		mainChar = nullptr;
		isDisplayed = false;
		currentEnemies = 0;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry()override;
	void drawBackground();
	~gameState();
	MainCharacter* getmainChar() const;
	Psyeb10Enemy* getEnemy() const;
	int getGameScore() const;
	bool loadGame();
	void saveGame();
protected:
	const int gridstartX = 350;
	const int gridstartY = 100;
	const int gridEndX = 950;
	const int gridEndY = 700;
	int gameScore;
	int lastTick;
	MainCharacter* mainChar;
	std::vector<Psyeb10Enemy*> enemyVec;
	bool isDisplayed;
	int currentEnemies;
};

