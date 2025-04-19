#pragma once
#include "header.h"
#include "Psyeb10States.h"
#include <vector>
#include "menuTileManager.h"
class gameSetupState :
    public Psyeb10States
{
public:
	gameSetupState(Psyeb10Engine* engineParam) :tm(50, 250, 1, 3) {
		this->engine = engineParam;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry() override;
	void keyPressed(int iKeyCode) override;
	~gameSetupState();
private:
	std::vector<DrawingSurface*> surfaces; // Store multiple drawing surfaces
	int currentFrame = 0;                  // Current frame index
	int animationDelay = 0; 
	DrawingSurface* previousSurface = nullptr; // Store the previous surface
	menuTileManager tm;
};

