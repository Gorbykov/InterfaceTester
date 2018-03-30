#ifndef FRAME_H
#define FRAME_H

#include <QString>
#include <QFile>
#include <QByteArray>

#define DEFAULT_SIZE 16
#define DEFAULT_DELAY 1000

class Frame
{
public:
    Frame();    
    Frame(QString frameName, QString fileName, int size, int delay);
    void setData(QByteArray data);
    void setFrameName(QString frameName);
    void setFileName(QString fileName);
	void setSize(int size);
	void setDelay(int delay);
	QByteArray getData();
	int getSize();
	int getDelay();
    QString getFrameName();
    QString getFileName();
    bool loadFile(QString fileName);
    bool saveFile(QString fileName);
	bool saveFile();
private:
    QString _fileName;
    QString _frameName;
	QByteArray _data;
	int _size;
	int _delay;
};

#endif // FRAME_H
