#pragma once
#include "header.h"
#include "Psyeb10States.h"
class mainMenuState : public Psyeb10States
{
public:
	mainMenuState(Psyeb10Engine* engineParam)  {
		this->engine = engineParam;
		screenOffsetX = 0;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
	void reEntry() override;
	void copyAllBackgroundBuffer() override;

private:
	int screenOffsetX;
};

