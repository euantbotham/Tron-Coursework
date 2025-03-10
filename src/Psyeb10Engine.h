#pragma once
#include "BaseEngine.h"
#include "Psyeb10TileManager.h"
#include "MainCharacter.h"
class MainCharacter;

class Psyeb10Engine :
    public BaseEngine
{
    friend class MainCharacter;
public:
    void virtSetupBackgroundBuffer() override;
    void virtDrawStringsOnTop() override;
    int virtInitialiseObjects() override;
protected:
    Psyeb10TileManager tm;
};

