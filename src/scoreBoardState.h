#pragma once
#include "Psyeb10Engine.h"
#include "Psyeb10States.h"
#include <string>
class scoreBoardState :
    public Psyeb10States
{
public:
	scoreBoardState(Psyeb10Engine* engine) {
		this->engine = engine;
		inputName = "";
	}
    void enter() override;
	void foreGroundStrings() override;
	void keyPressed(int iKeyCode) override;

private:
	std::string inputName;

};

