#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include "frameIn.h"
#include <QtCore>

#define TIMEOUT 10000

class EthernetController : public QObject
{
    Q_OBJECT
public:
    explicit EthernetController(QObject *parent = nullptr);
    ~EthernetController();
    void read(FrameIn* currentFrameIn);
    void write(QByteArray* data);

    bool startSession();
    void endSession();

    bool isValid();

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
    QTimer _timer;
    FrameIn* _currentFrameIn;

private slots:
    void handleReadyRead();
    void handleTimeout();

};

#endif // ETHERNETCONTROLLER_H
