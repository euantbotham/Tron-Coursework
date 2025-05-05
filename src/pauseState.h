#pragma once
#include "header.h"
#include "Psyeb10States.h"
#include "menuTileManager.h"
#include "bouncingLogo.h"

class pauseState :
    public Psyeb10States
{
public:
	pauseState(Psyeb10Engine* engineParam) : tm(50,250,1,4) {
		this->engine = engineParam;
		isdisplayed = false;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry() override;
	~pauseState() override;
protected:
	menuTileManager tm;
	bouncingLogo* logo;
	bool isdisplayed;

};

