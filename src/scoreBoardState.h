#pragma once
#include "Psyeb10Engine.h"
#include "Psyeb10States.h"
#include <string>
#include "scoreBoardFilterPoints.h"

class scoreBoardState : public Psyeb10States
{
public:
    scoreBoardState(Psyeb10Engine* engine, int score);
    void enter() override;
    void foreGroundStrings() override;
    void keyPressed(int iKeyCode) override;
    void mouseWheelScrolled(int x, int y, int which, int timestamp) override;
private:
    std::string inputName;
    ScoreBoardScaling m_filterScaling;           // For zooming with limits
    ScoreBoardTranslation m_filterTranslation;   // For panning with boundaries
	std::vector<std::string> scoreBoardEntries; // Placeholder for actual scoreboard entries
	std::vector<int> scoreBoardScores; // Placeholder for actual scores
    int playerscore;
    int playerPos;
    bool nameSaved;
    void loadScore();
    void saveScore();
};