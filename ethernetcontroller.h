#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

#include <QObject>
#include <QUdpSocket>
#include <QtCore>
#include "frame.h"

//#define TIMEOUT 10000


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
    void read(Frame* currentFrame);
    void write(Frame* currentFrame);

    bool startInOutSession();
    bool startInSession();
    bool startOutSession();
    void endInOutSession();
    void endInSession();
    void endOutSession();
    void endSession();

    bool isInReady();
    bool isOutReady();

    FullAddress* getTPCAddress();
    FullAddress* getRPCAddress();
    FullAddress* getTDevAddress();
    FullAddress* getRDevAddress();
    int getTimeout();
    void setTimeout(int timeout);
    void setTSocket(FullAddress *tPCAddress, FullAddress *rDevAddress);
    void setRSocket(FullAddress *tDevAddress, FullAddress *rPCAddress);
signals:
    void refreshFrameOut();
    void refreshSendingStatus(bool status);

private:
    QUdpSocket *_tPCSocket = nullptr;
    FullAddress *_tPCAddress = nullptr;
    QUdpSocket *_rPCSocket = nullptr;
    FullAddress *_rPCAddress = nullptr;
    FullAddress *_tDevAddress = nullptr;
    FullAddress *_rDevAddress = nullptr;
    QVector<QByteArray*>* _readData = nullptr;
    QVector<int>* _readDelays = nullptr;
    QTimer _timer;
    Frame* _currentFrame;
    int _delaysPointer = 0;
    int _timeout = 10000;
    bool _inReady = false;
    bool _outReady = false;

private slots:
    void writePacket();
    void handleReadyRead();
    void handleTimeout();

};


#endif // ETHERNETCONTROLLER_H
