#pragma once
#include "Psyeb10Engine.h"
#include "Psyeb10States.h"
#include <string>
#include "scoreBoardFilterPoints.h"
#include "ExampleFilterPointClasses.h"
class scoreBoardState :
    public Psyeb10States
{
public:
	scoreBoardState(Psyeb10Engine* engine) : m_filterScaling(0, 0, engine), m_filterTranslation(0, 0, &m_filterScaling) {
		this->engine = engine;
		inputName = "";
	}
    void enter() override;
	void foreGroundStrings() override;
	void keyPressed(int iKeyCode) override;
	void mouseWheelScrolled(int x, int y, int which, int timestamp) override;

private:
	std::string inputName;
	FilterPointsScaling m_filterScaling;
	FilterPointsTranslation m_filterTranslation;

};

