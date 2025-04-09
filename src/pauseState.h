#pragma once
#include "header.h"
#include "Psyeb10States.h"
class pauseState :
    public Psyeb10States
{
public:
	pauseState(Psyeb10Engine* engineParam) {
		this->engine = engineParam;
	}
	void enter() override;
	void foreGroundStrings() override;
	void initObjects() override;
	void reset() override;
	void mouseDown(int iButton, int iX, int iY) override;
	void mainLoopPreUpdate() override;
};

