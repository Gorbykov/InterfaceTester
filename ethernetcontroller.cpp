#include "ethernetcontroller.h"
#include <QDebug>

EthernetController::EthernetController(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(refreshFrameOut()), parent, SLOT(refreshFrameOut()));
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

void EthernetController::write(Frame *currentFrame)
{
    if (!isInReady())
    {
        return;
    }
    _currentFrame = currentFrame;
    _delaysPointer = 0;
    if(_currentFrame->getDelaysIn() != nullptr && !_currentFrame->getDelaysIn()->isEmpty())
    {
        QTimer::singleShot(_currentFrame->getDelaysIn()->at(_delaysPointer),Qt::PreciseTimer,this,SLOT(writePacket()));
    }
    //    for (int i =0; i<currentFrame->getDelaysIn()->size();i++)
    //    {
    //        QTimer::singleShot(_currentFrame->getDelaysIn()->at(i),Qt::PreciseTimer,[=]{
    //            _tSocket->writeDatagram(*(_currentFrame->getDataIn()->at(i)),_rAddress->ip,_rAddress->port);
    //        });
    //    }
}

void EthernetController::writePacket()
{
    _tSocket->writeDatagram(*(_currentFrame->getDataIn()->at(_delaysPointer)),_rAddress->ip,_rAddress->port);
    _delaysPointer++;
    if(_delaysPointer<_currentFrame->getDelaysIn()->size())
    {
        QTimer::singleShot(_currentFrame->getDelaysIn()->at(_delaysPointer),Qt::PreciseTimer,this,SLOT(writePacket()));
    }
}

void EthernetController::read(Frame *currentFrame)
{
    if (!isOutReady())
    {
        return;
    }
    _currentFrame = currentFrame;
    delete _readData;
    delete _readDelays;
    _readDelays = new QVector<int>();
    _currentFrame->setDelaysOut(_readDelays);
    _readData = new QVector<QByteArray*>();
    _currentFrame->setDataOut(_readData);
    _timer.setSingleShot(true);
    _timer.start(TIMEOUT);
}


void EthernetController::handleReadyRead()
{
    //    _readData->push_back(new QByteArray(_currentSerialPort->readAll()));
    //    _currentFrame->setDataOut(_readData);
    //    _readDelays->push_back(TIMEOUT - _timer.remainingTime());
    //    _currentFrame->setDelaysOut(_readDelays);
    //    emit refreshFrameIn();
    //    _timer.start(TIMEOUT);
    _readData->push_back(new QByteArray(_rSocket->pendingDatagramSize(),'/0'));
    _rSocket->readDatagram(_readData->back()->data(),_readData->back()->size());
    _currentFrame->setDataOut(_readData);
    _readDelays->push_back(TIMEOUT - _timer.remainingTime());
    _timer.start(TIMEOUT);
    _currentFrame->setDelaysOut(_readDelays);
    emit refreshFrameOut();
    //qDebug()<<(QString::fromUtf8( _readData))<<"/n";
}

void EthernetController::handleTimeout()
{

    //handleReadyRead();
    //disconnect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
    //endSession();
}
