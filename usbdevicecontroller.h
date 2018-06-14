#ifndef USBDEVICECONTROLLER_H
#define USBDEVICECONTROLLER_H
#include <QtCore>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>
#include <QTextEdit>
#include "frame.h"

//#include "mainwindow.h"


#define TIMEOUT 5000

class UsbDeviceController: public QObject
{
    Q_OBJECT

public:

    UsbDeviceController(QWidget *parent);
    ~UsbDeviceController();

    bool startSession();
    bool startInSession();
    bool startOutSession();
    void endSession();
    bool isReady();

    QSerialPort* getDevice();
    void read(Frame* currentFrame);
    void write(Frame *currentFrame);
    void setDevice(QSerialPort* dev);
    QList<QSerialPortInfo> getDeviceList();

signals:
    void refreshFrameOut();

private:
    QList<QSerialPortInfo> _deviceList;
    QTimer _timer;
    QTimer _writeTiemr;
    QTimer _readTiemr;
    QSerialPort *_currentSerialPort  = nullptr;
    QVector<QByteArray*>* _readData = nullptr;
    QVector<int> *_readDelays = nullptr;
    Frame* _currentFrame = nullptr;
    bool _ready = false;

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

  };

#endif // USBDEVICECONTROLLER_H
