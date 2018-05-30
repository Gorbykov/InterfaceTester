#include "ethernetcontroller.h"
#include <QDebug>

EthernetController::EthernetController(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(refreshFrameIn()), parent, SLOT(refreshFrameIn()));
    connect(&_timer, &QTimer::timeout, this, &EthernetController::handleTimeout);
}

EthernetController::~EthernetController()
{
    endSession();
}

void EthernetController::setTSocket(QUdpSocket *tSocket)
{
    _tSocket = tSocket;
}

void EthernetController::setRSocket(QUdpSocket *rSocket)
{
    _rSocket = rSocket;
    connect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
}

QUdpSocket* EthernetController::getTSocket()
{
    return _tSocket;
}

QUdpSocket* EthernetController::getRSocket()
{
    return _rSocket;
}

bool EthernetController::startSession()
{
    if (_tSocket->isValid() && _rSocket->isValid())
    {
        //_tSocket->disconnectFromHost();
        _tSocket->connectToHost(_rSocket->localAddress(),_rSocket->localPort(),QIODevice::WriteOnly);
        _tSocket->waitForConnected();
        qDebug() << (_tSocket->localAddress().toString())<<" to "<<(_tSocket->peerAddress().toString())<<"/n";


        //_tSocket->disconnectFromHost();
        //_rSocket->connectToHost(_tSocket->localAddress(),_tSocket->localPort(),QIODevice::ReadWrite);
        //_rSocket->waitForConnected();
        //qDebug() << (_rSocket->localAddress().toString())<<" to "<<(_rSocket->peerAddress().toString())<<"/n";

        return true;
    }
    else
    {
        return false;
    }
}

bool EthernetController::isValid()
{
    if (_tSocket!=nullptr && _rSocket!=nullptr)
        if (_tSocket->isValid() && _rSocket->isValid())
        {
            return true;
        }
        else
        {
            return false;
        }
    else
    {
        return false;
    }
}

void EthernetController::endSession()
{
    _tSocket->disconnectFromHost();
    _rSocket->disconnectFromHost();
}

void EthernetController::read(FrameIn* currentFrameIn)
{
    qDebug("read");
    qDebug()<<(_rSocket->localAddress().toString())
           <<" to "
           <<(_rSocket->peerAddress().toString())
           <<"/n";
    _currentFrameIn = currentFrameIn;
    _readData.clear();
    _currentFrameIn->setData(_readData);
    _timer.setSingleShot(true);
    _timer.start(TIMEOUT);
}

void EthernetController::write(QByteArray* data)
{
    _tSocket->write(*data);
    _tSocket->waitForBytesWritten();
    qDebug() << (_tSocket->localAddress().toString())<<" to "<<(_tSocket->peerAddress().toString())<<"/n";
}


void EthernetController::handleReadyRead()
{
    _readData.resize(_rSocket->pendingDatagramSize());
    _rSocket->readDatagram(_readData.data(),_readData.size());
    _currentFrameIn->setData(_readData);
    emit refreshFrameIn();
    if (!_timer.isActive())
        _timer.start(TIMEOUT);
    qDebug()<<(QString::fromUtf8( _readData))<<"/n";
}

void EthernetController::handleTimeout()
{

    //handleReadyRead();
    //disconnect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
    //endSession();
}
