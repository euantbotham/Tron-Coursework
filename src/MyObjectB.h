#pragma once
#include "DisplayableObject.h"
class MyObjectB :
    public DisplayableObject
{
public:
    MyObjectB(BaseEngine* pEngine) : DisplayableObject(100,200,pEngine,100,200,true)
    {

    }
    void virtDraw();
    void virtDoUpdate(int iCurrentTime);
};
