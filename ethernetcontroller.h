#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include "frameIn.h"

class EthernetController : public QObject
{
    Q_OBJECT
public:
    explicit EthernetController(QObject *parent = nullptr);
    void read(FrameIn* currentFrameIn);
    void write(QByteArray* data);

    bool startSession();
    void endSession();

    QUdpSocket* getTSocket();
    QUdpSocket* getRSocket();
    void setTSocket(QUdpSocket* tSocket);
    void setRSocket(QUdpSocket* rSocket);
signals:
    void refreshFrameIn();

private:
    QUdpSocket *_tSocket = nullptr;
    QUdpSocket *_rSocket = nullptr;
    QByteArray _readData;

};

#endif // ETHERNETCONTROLLER_H
