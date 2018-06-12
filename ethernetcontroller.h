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
    void endInSession();
    void endOutSession();

    bool isInReady();
    bool isOutReady();

    QUdpSocket* getTSocket();
    QUdpSocket* getRSocket();
    FullAddress* getTAddress();
    FullAddress* getRAddress();
    void setTSocket( FullAddress *tAddress);
    void setRSocket( FullAddress *rAddress);
signals:
    void refreshFrameIn();

private:
    QUdpSocket *_tSocket = nullptr;
    FullAddress *_tAddress = nullptr;
    FullAddress *_rAddress = nullptr;
    QUdpSocket *_rSocket = nullptr;
    QByteArray _readData;
    QTimer _timer;
    FrameIn* _currentFrameIn;
    bool _inReady = false;
    bool _outReady = false;

private slots:
    void handleReadyRead();
    void handleTimeout();

};


#endif // ETHERNETCONTROLLER_H
