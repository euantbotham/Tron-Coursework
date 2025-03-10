#pragma once
#include "BaseEngine.h"
class Psyeb10Engine :
    public BaseEngine
{

public:
    void virtSetupBackgroundBuffer() override;
    void virtDrawStringsOnTop() override;
    int virtInitialiseObjects() override;
};

