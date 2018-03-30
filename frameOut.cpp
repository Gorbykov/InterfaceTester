#include "frameOut.h"

FrameOut::FrameOut(QString frameName, QString fileName, int size, int delay)
{

    this->setFrameName(frameName);
    this->setFileName(fileName);
    this->setSize(size);
    this->setDelay(delay);
    loadFile(fileName);
}

FrameOut::~FrameOut()
{
}
