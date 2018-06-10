#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include "frameIn.h"
#include <QtCore>

#define TIMEOUT 10000


struct FullAddress
{
  QHostAddress ip;
  quint16 port;
  FullAddress(QHostAddress ip, quint16 port)
  {
      this->ip = ip;
      this->port = port;
  }
};

class EthernetController : public QObject
{
    Q_OBJECT
public:
    explicit EthernetController(QObject *parent = nullptr);
    ~EthernetController();
    void read(FrameIn* currentFrameIn);
    void write(QByteArray* data);

    bool startSession();
    bool startInSession();
    bool startOutSession();
    void endSession();

    bool isValid();

    QUdpSocket* getTSocket();
    QUdpSocket* getRSocket();
    FullAddress* getTAddress();
    FullAddress* getRAddress();
    void setTSocket(QUdpSocket* tSocket, FullAddress *tAddress);
    void setRSocket(QUdpSocket* rSocket, FullAddress *rAddress);
signals:
    void refreshFrameIn();

private:
    QUdpSocket *_tSocket = nullptr;
    FullAddress *_tAddress;
    FullAddress *_rAddress;
    QUdpSocket *_rSocket = nullptr;
    QByteArray _readData;
    QTimer _timer;
    FrameIn* _currentFrameIn;

private slots:
    void handleReadyRead();
    void handleTimeout();

};


#endif // ETHERNETCONTROLLER_H
