#include "ethernetcontroller.h"

EthernetController::EthernetController(QObject *parent) : QObject(parent)
{

}

void EthernetController::setTSocket(QUdpSocket *tSocket)
{
    _tSocket = tSocket;
}

void EthernetController::setRSocket(QUdpSocket *rSocket)
{
    _rSocket = rSocket;
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

}

void EthernetController::endSession()
{

}
