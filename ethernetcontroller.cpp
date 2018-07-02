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

void EthernetController::setTSocket(FullAddress *tPCAddress,FullAddress *rDevAddress)
{
    _tPCAddress = tPCAddress;
    _rDevAddress = rDevAddress;
}

void EthernetController::setRSocket(FullAddress *tDevAddress, FullAddress *rPCAddress)
{
    _rPCAddress = rPCAddress;
    _tDevAddress = tDevAddress;
}

void EthernetController::setTimeout(int timeout)
{
    _timeout = timeout;
}

int EthernetController::getTimeout()
{
    return _timeout;
}

FullAddress* EthernetController::getTPCAddress()
{
    return _tPCAddress;
}

FullAddress* EthernetController::getRPCAddress()
{
    return _rPCAddress;
}

FullAddress* EthernetController::getTDevAddress()
{
    return _tDevAddress;
}

FullAddress* EthernetController::getRDevAddress()
{
    return _rDevAddress;
}

bool EthernetController::startInSession()
{
    this->endInSession();
    _tPCSocket =  new QUdpSocket(parent());
    if(_tPCAddress == nullptr)
    {
        return false;
    }
    if(_rDevAddress == nullptr)
    {
        _rDevAddress = new FullAddress(QHostAddress::Broadcast,65535);
    }
    _tPCSocket->bind(_tPCAddress->ip,_tPCAddress->port);
    if (_tPCSocket->isValid())
    {
        _tPCAddress->port = _tPCSocket->localPort();
        _inReady = true;
        return true;
    }
    return false;
}

bool EthernetController::startOutSession()
{
    this->endOutSession();
    _rPCSocket =  new QUdpSocket(parent());
    if(_rPCAddress == nullptr)
    {
        return false;
    }
    connect(_rPCSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
    _rPCSocket->bind(_rPCAddress->ip,_rPCAddress->port);
    if (_rPCSocket->isValid())
    {
        _rPCAddress->port = _rPCSocket->localPort();
        _outReady = true;
        return true;
    }
    return false;
}

bool EthernetController::startInOutSession()
{

}


bool EthernetController::isInReady()
{
    return (_tPCSocket != nullptr) && _inReady;
}

bool EthernetController::isOutReady()
{
    return (_rPCSocket != nullptr) && _outReady;
}

void EthernetController::endSession()
{
    endInSession();
    endOutSession();
}

void EthernetController::endInSession()
{
    if (_tPCSocket != nullptr)
    {
        _tPCSocket->close();
        delete _tPCSocket;
        _tPCSocket = nullptr;
    }
    _inReady = false;
}

void EthernetController::endOutSession()
{
    if(_rPCSocket != nullptr)
    {
        disconnect(_rPCSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);

        _rPCSocket->close();
        delete _rPCSocket;
        _rPCSocket = nullptr;
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
    _tPCSocket->writeDatagram(*(_currentFrame->getDataIn()->at(_delaysPointer)),_rDevAddress->ip,_rDevAddress->port);
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
    emit refreshSendingStatus(true);
    _timer.start(_timeout);
}


void EthernetController::handleReadyRead()
{
    //    _readData->push_back(new QByteArray(_currentSerialPort->readAll()));
    //    _currentFrame->setDataOut(_readData);
    //    _readDelays->push_back(TIMEOUT - _timer.remainingTime());
    //    _currentFrame->setDelaysOut(_readDelays);
    //    emit refreshFrameIn();
    //    _timer.start(TIMEOUT);
    _readData->push_back(new QByteArray(_rPCSocket->pendingDatagramSize(),'/0'));
    _rPCSocket->readDatagram(_readData->back()->data(),_readData->back()->size());
    _currentFrame->setDataOut(_readData);
    _readDelays->push_back(_timeout - _timer.remainingTime());
    _timer.start(_timeout);
    _currentFrame->setDelaysOut(_readDelays);
    emit refreshFrameOut();
    //qDebug()<<(QString::fromUtf8( _readData))<<"/n";
}

void EthernetController::handleTimeout()
{
    emit refreshSendingStatus(false);
    //handleReadyRead();
    //disconnect(_rSocket, &QUdpSocket::readyRead, this, &EthernetController::handleReadyRead);
    //endSession();
}
