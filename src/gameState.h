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
		currentLevel = 1;
		collisionsEnabled = true;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry()override;
	void drawBackground();
	void recieveUpdate(int code)override;
	~gameState();
	MainCharacter* getmainChar() const;
	int getGameScore() const;
	bool loadGame();
	void saveGame();
	void cleanTileManager(int code);
	void setCollisions(bool collision);
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
	int currentLevel;
private:
	bool checkPixelPerfectCollision(SimpleImage& bike1, int x1, int y1, int r1,
		SimpleImage& bike2, int x2, int y2, int r2);
	bool checkBikeCollision(SimpleImage& bike1, int x1, int y1, int r1,
		SimpleImage& bike2, int x2, int y2, int r2);
	void applyRotation(int& x, int& y, int width, int height, int rotation);

	bool collisionsEnabled;

};


