#pragma once
#include "frame.h"
class FrameOut :
	public Frame
{
public:
    FrameOut(QString frameName, QString fileName, int size, int delay);
	~FrameOut();
};
