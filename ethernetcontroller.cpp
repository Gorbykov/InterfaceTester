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

void EthernetController::setTSocket(QUdpSocket *tSocket, FullAddress *tAddress)
{
    _tSocket = tSocket;
    _tAddress = tAddress;
}

void EthernetController::setRSocket(QUdpSocket *rSocket, FullAddress *rAddress)
{
    _rSocket = rSocket;
    _rAddress = rAddress;
    connect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
}

FullAddress* EthernetController::getTAddress()
{
    return _tAddress;
}

FullAddress* EthernetController::getRAddress()
{
    return _rAddress;
}

QUdpSocket* EthernetController::getTSocket()
{
    return _tSocket;
}

QUdpSocket* EthernetController::getRSocket()
{
    return _rSocket;
}

bool EthernetController::startInSession()
{
    this->endSession();
    _tSocket =  new QUdpSocket(parent());
    _tSocket->bind(_tAddress->ip,_tAddress->port);
    if (_tSocket->isValid())
    {
        _tAddress->port = _tSocket->localPort();
        return true;
    }
    return false;
}

bool EthernetController::startOutSession()
{
    this->endSession();
    _rSocket =  new QUdpSocket(parent());
    _rSocket->bind(_rAddress->ip,_rAddress->port);
    if (_rSocket->isValid())
    {
        _rAddress->port = _rSocket->localPort();
        return true;
    }
    return false;
}

bool EthernetController::startSession()
{
    return startInSession() && startOutSession();
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
    disconnect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
    _tSocket->disconnectFromHost();
    _tSocket->abort();
    _rSocket->disconnectFromHost();
    _rSocket->abort();
}

void EthernetController::read(FrameIn* currentFrameIn)
{
    _currentFrameIn = currentFrameIn;
    _readData.clear();
    _currentFrameIn->setData(_readData);
    _timer.setSingleShot(true);
    _timer.start(TIMEOUT);
}

void EthernetController::write(QByteArray* data)
{
    _tSocket->writeDatagram(*data,_rAddress->ip,_rAddress->port);
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
