#pragma once
#include "frame.h"
class FrameIn :
	public Frame
{
public:
    FrameIn(QString frameName, QString fileName, int size, int delay);
	virtual ~FrameIn();
};
