#include "frame.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

Frame::Frame()
{
}

Frame::Frame(QString folderName)
{
    Frame();
    _folderName = folderName;
}


void Frame::setDataIn(QVector<QByteArray*> *dataIn)
{
    _dataIn = dataIn;
}

void Frame::setDelaysIn(QVector<int> *delaysIn)
{
    _delaysIn = delaysIn;
}

void Frame::setDataOut(QVector<QByteArray*> *dataOut)
{
    _dataOut = dataOut;
}

void Frame::setDelaysOut(QVector<int> *delaysOut)
{
    _delaysOut = delaysOut;
}


void Frame::setFolderName(QString folderName)
{
    _folderName = folderName;
}


void Frame::setSizesIn(QVector<int> *sizesIn)
{
    if(_fullDataIn == nullptr)
    {
        return;
    }
}

void Frame::setFullDataIn(QByteArray *dataIn)
{
    _fullDataIn = dataIn;
}

QByteArray* Frame::getFullDataIn()
{
    return _fullDataIn;
}

QVector<QByteArray*>* Frame::getDataIn()
{
    return _dataIn;
}

QVector<int> *Frame::getDelaysIn()
{
    return _delaysIn;
}

QVector<QByteArray*>* Frame::getDataOut()
{
    return _dataOut;
}

QVector<int> *Frame::getDelaysOut()
{
    return _delaysOut;
}


QVector<int>* Frame::getSizesIn()
{
    QVector<int>* sizes = new QVector<int>();
    for (int i = 0; i < _dataIn->size(); i++) {
        sizes->push_back(_dataIn->at(i)->size());
    }
    return sizes;
}

QVector<int>* Frame::getSizesOut()
{
    QVector<int>* sizes = new QVector<int>();
    for (int i = 0; i < _dataOut->size(); i++) {
        sizes->push_back(_dataOut->at(i)->size());
    }
    return sizes;
}

QString Frame::getFolderName()
{
    return _folderName;
}

QVector<int>* Frame::getFullSizesIn()
{
    return _fullSizesIn;
}

QVector<int>* Frame::getFullDelaysIn()
{
    return _fullDelaysIn;
}

bool Frame::saveAll()
{
    return saveAll(_folderName);
}

bool Frame::saveAll(QString folderName)
{
    return saveIn(folderName) && saveOut(folderName);
}

bool Frame::saveIn()
{
    return saveIn(_folderName);
}

bool Frame::saveIn(QString folderName)
{
    return _save(folderName,Direction::in);
}

bool Frame::saveOut()
{
    return saveOut(_folderName);
}

bool Frame::saveOut(QString folderName)
{
    return _save(folderName,Direction::out);
}
///////////////

bool Frame::openAll()
{
    return openAll(_folderName);
}

bool Frame::openAll(QString folderName)
{
    return openIn(folderName) && openOut(folderName);
}

bool Frame::openIn()
{
    return openIn(_folderName);
}

bool Frame::openIn(QString folderName)
{
    return _open(folderName,Direction::in);
}

bool Frame::openOut()
{
    return openOut(_folderName);
}

bool Frame::openOut(QString folderName)
{
    return _open(folderName, Direction::out);
}

////////////////////

bool Frame::_save(QString folderName, Direction direction)
{
    bool ok;
    QString dataFileName;
    QString delaysFileName;
    QVector<int> *delays;
    QVector<int> *sizes;
    QVector<QByteArray*> *data;
    if(direction == Direction::in)
    {
        dataFileName = DEFAULT_IN;
        delaysFileName = DEFAULT_IN_DELAYS;
        if (_fullDelaysIn != nullptr && _fullDataIn != nullptr)
        {
            delays = _fullDelaysIn;
            data = new QVector<QByteArray*>();
            data->push_back(_fullDataIn);
            sizes = _fullSizesIn;
        }
    }
    else if(direction == Direction::out)
    {
        dataFileName = DEFAULT_OUT;
        delaysFileName = DEFAULT_OUT_DELAYS;
        if (_delaysOut != nullptr && _dataOut != nullptr)
        {
            delays = _delaysOut;
            data = _dataOut;
            sizes = getSizesOut();
        }
    }

    qDebug() << QDir::currentPath();
    if (!QDir::setCurrent(folderName))
       QMessageBox::warning(0,"Ошибка открытия папки","Невозможно открыть папку");
    qDebug() << QDir::currentPath();

    QFile dataFile(dataFileName);
    QFile delaysFile(delaysFileName);

    ok = dataFile.open(QIODevice::WriteOnly);
    if (!ok)
    {
        return ok;
    }
    ok = delaysFile.open(QIODevice::WriteOnly);
    if (!ok)
    {
        return ok;
    }
    QDataStream dataStream(&dataFile);
    QTextStream delaysStream(&delaysFile);

    for(int i = 0; i<data->size(); i++)
    {
        dataStream.writeRawData(data->at(i)->data(),data->at(i)->size());
    }
    for(int i = 0; i<sizes->size(); i++)
    {
        delaysStream << (*sizes)[i] << " " << (*delays)[i] << "\r\n";
    }

    dataFile.close();
    delaysFile.close();
    return ok;

}

bool Frame::_open(QString folderName,  Direction direction)
{
    bool ok;
    QString dataFileName;
    QString delaysFileName;
    QVector<int> *delays;
    QVector<QByteArray*> *data;
    if(direction == Direction::in)
    {
        dataFileName = DEFAULT_IN;
        delaysFileName = DEFAULT_IN_DELAYS;
    }
    else if(direction == Direction::out)
    {
        dataFileName = DEFAULT_OUT;
        delaysFileName = DEFAULT_OUT_DELAYS;
    }
    delays = new QVector<int>();
    data = new QVector<QByteArray*>();

    qDebug() << QDir::currentPath();
    if (!QDir::setCurrent(folderName))
       QMessageBox::warning(0,"Ошибка открытия папки","Невозможно открыть папку");
    qDebug() << QDir::currentPath();

    QFile dataFile(dataFileName);
    QFile delaysFile(delaysFileName);

    ok = dataFile.open(QIODevice::ReadWrite);
    if (!ok)
    {
        return ok;
    }
    ok = delaysFile.open(QIODevice::ReadWrite);
    if (!ok)
    {
        return ok;
    }

    QByteArray dataAll = dataFile.readAll();
    if(direction == Direction::in)
    {
        _fullDataIn = new QByteArray(dataAll);
    }
    QTextStream delaysStram(&delaysFile);
    QVector<int> sizes;

    while (!delaysStram.atEnd())
    {
        QString line = delaysStram.readLine();
        QStringList parameters =  line.split(' ');
        sizes.push_back(parameters.at(0).toInt());
        delays->push_back(parameters.at(1).toInt());
    }
    if(direction == Direction::in)
    {
        _fullDelaysIn= delays;
        _fullSizesIn = new QVector<int> (sizes);
    }
    int p = 0;
    int i = 0;
    for (i = 0; (i<sizes.size());i++)
    {
        if (p+sizes[i] > dataAll.size())
        {
            break;
        }
        data->push_back(new QByteArray(dataAll.mid(p,sizes[i])));
        p +=sizes[i];
    }
    sizes = sizes.mid(0,i);
    delays = new QVector<int>(delays->mid(0,i));
    dataFile.close();
    delaysFile.close();
    if(direction == Direction::in)
    {
        delete _delaysIn;
        delete _dataIn;
        _delaysIn = delays;
        _dataIn = data;
    }
    if(direction == Direction::out)
    {
        delete _delaysOut;
        delete _dataOut;
        _delaysOut = delays;
        _dataOut = data;
    }
    return ok;
}
