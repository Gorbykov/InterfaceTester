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

void EthernetController::setTSocket( FullAddress *tAddress)
{
    //_tSocket = tSocket;
    _tAddress = tAddress;
}

void EthernetController::setRSocket(FullAddress *rAddress)
{
    //_rSocket = rSocket;
    _rAddress = rAddress;
    //connect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
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
    this->endInSession();
    _tSocket =  new QUdpSocket(parent());
    _tSocket->bind(_tAddress->ip,_tAddress->port);
    if (_tSocket->isValid())
    {
        _tAddress->port = _tSocket->localPort();
        _inReady = true;
        return true;
    }
    return false;
}

bool EthernetController::startOutSession()
{
    this->endOutSession();
    _rSocket =  new QUdpSocket(parent());
    connect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
    _rSocket->bind(_rAddress->ip,_rAddress->port);
    if (_rSocket->isValid())
    {
        _rAddress->port = _rSocket->localPort();
        _outReady = true;
        return true;
    }
    return false;
}

bool EthernetController::startSession()
{
    return startInSession() && startOutSession();
}


bool EthernetController::isInReady()
{
    return (_tSocket != nullptr) && _inReady;
}

bool EthernetController::isOutReady()
{
    return (_rSocket != nullptr) && _outReady;
}

void EthernetController::endSession()
{
    endInSession();
    endOutSession();
}

void EthernetController::endInSession()
{
    if (_tSocket != nullptr)
    {
        _tSocket->close();
        delete _tSocket;
        _tSocket = nullptr;
    }
    _inReady = false;
}

void EthernetController::endOutSession()
{
    if(_rSocket != nullptr)
    {
        disconnect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);

        _rSocket->close();
        delete _rSocket;
        _rSocket = nullptr;
    }
    _outReady = false;
}
void EthernetController::read(FrameIn* currentFrameIn)
{
    if (!isInReady())
    {
        return;
    }
    _currentFrameIn = currentFrameIn;
    _readData.clear();
    _currentFrameIn->setData(_readData);
    _timer.setSingleShot(true);
    _timer.start(TIMEOUT);
}

void EthernetController::write(QByteArray* data)
{
    if (!isOutReady())
    {
        return;
    }
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
