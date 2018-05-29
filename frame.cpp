#include "frame.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Frame::Frame()
{
    _data = new QByteArray("");
}

Frame::Frame(QString frameName, QString fileName, int size, int delay)
{
    Frame();
    this->setFrameName(frameName);    
    this->setFileName(fileName);
    this->setSize(size);
    this->setDelay(delay);
}


bool Frame::loadFile(QString fileName)
{
    QString dataFileName = fileName + ".hex";
    QString settingsFileName = fileName + ".conf";
    QFile dataFile(dataFileName);
    QFile settingsFile(settingsFileName);
    bool settingsOpenStatus;
    if (_size==-1 || _delay==-1)
    {
        settingsOpenStatus = settingsFile.open(QIODevice::ReadOnly);
        if (!settingsOpenStatus)
        {
            QMessageBox::information(0, "Ошибка открытия файла", settingsFile.errorString());
            bool dataOpenStatus = settingsFile.open(QIODevice::ReadWrite);
            QMessageBox::information(0,"","Создан новый пустой файл: " + settingsFileName);
            if (!dataOpenStatus)
            {
                QMessageBox::information(0, "Ошибка открытия файла", dataFile.errorString());
            }
            else
            {
                _size = DEFAULT_SIZE;
                _delay = DEFAULT_DELAY;
                QTextStream settingsFileStream(&settingsFile);
                settingsFileStream << _size << '\n' << _delay;
            }
        }
        else
        {            
            bool *ok = new bool(true);
            _size = settingsFile.readLine().toInt(ok);
            if(! *ok ) _size = DEFAULT_SIZE;
            _delay = settingsFile.readLine().toInt(ok);
            if(! *ok ) _delay = DEFAULT_DELAY;
        }
    }
    else
    {
        settingsOpenStatus = settingsFile.open(QIODevice::ReadWrite);
        if (!settingsOpenStatus)
        {
            QMessageBox::information(0, "Ошибка открытия файла", settingsFile.errorString());
        }
        else
        {
            QTextStream settingsFileStream(&settingsFile);
            settingsFileStream << _size << '\n' << _delay;
        }
    }
    bool dataOpenStatus = dataFile.open(QIODevice::ReadOnly);
    if (!dataOpenStatus) {

        bool dataOpenStatus = dataFile.open(QIODevice::ReadWrite);
        QMessageBox::information(0,"","Создан новый пустой файл: " + dataFileName);
        if (!dataOpenStatus)
        {
            QMessageBox::information(0, "Ошибка открытия файла", dataFile.errorString());
        }
	}
	else
	{
        _data = new QByteArray(dataFile.readAll());
	}
    dataFile.close();
    settingsFile.close();
    return dataOpenStatus||settingsOpenStatus;
}

bool Frame::saveFile(QString fileName)
{    
    QString dataFileName = fileName + ".hex";
    QFile file(dataFileName);
	bool openStatus = file.open(QIODevice::WriteOnly);
	if (!openStatus) {
        QMessageBox::information(0, "File save error", file.errorString());
	}
	else
	{
        file.write(*_data);
	}
	file.close();
	return openStatus;
}

bool Frame::saveFile()
{
    return saveFile(_fileName);
}

void Frame::setData(QByteArray data)
{
    _data = new QByteArray(data);
}

void Frame::setFrameName(QString frameName)
{
    _frameName = frameName;
}

void Frame::setFileName(QString fileName)
{
    _fileName = fileName;
}


void Frame::setDelay(int delay)
{
	_delay = delay;
}

void Frame::setSize(int size)
{
	_size = size;
}

QByteArray Frame::getData()
{
    return *_data;
}

QString Frame::getFrameName()
{
    return _frameName;
}

QString Frame::getFileName()
{
    return _fileName;
}

int Frame::getDelay()
{
	return _delay;
}

int Frame::getSize()
{
	return _size;
}

bool Frame::isEmpty()
{
    return _data->isEmpty();
}
