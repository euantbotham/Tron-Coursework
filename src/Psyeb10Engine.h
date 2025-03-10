#pragma once
#include "BaseEngine.h"
#include "Psyeb10TileManager.h"
class Psyeb10Engine :
    public BaseEngine
{

public:
    void virtSetupBackgroundBuffer() override;
    void virtDrawStringsOnTop() override;
    int virtInitialiseObjects() override;
protected:
    Psyeb10TileManager tm;
};

