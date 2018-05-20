#pragma once
#include "frame.h"
class FrameIn :
	public Frame
{
public:
    FrameIn(QString frameName, QString fileName, int size=0, int delay=0);
	virtual ~FrameIn();
};
