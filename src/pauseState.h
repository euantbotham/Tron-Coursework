#pragma once
#include "header.h"
#include "Psyeb10States.h"
#include "menuTileManager.h"


class pauseState :
    public Psyeb10States
{
public:
	pauseState(Psyeb10Engine* engineParam) : tm(100,100,5,5) {
		this->engine = engineParam;
		
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry() override;

protected:
	menuTileManager tm;
};

