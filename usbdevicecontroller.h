#ifndef USBDEVICECONTROLLER_H
#define USBDEVICECONTROLLER_H
#include <QtCore>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>
#include "frameIn.h"
#include <QTextEdit>

//#include "mainwindow.h"


#define TIMEOUT 5000

class UsbDeviceController: public QObject
{
    Q_OBJECT

public:

    UsbDeviceController(QWidget *parent);
    ~UsbDeviceController();
    bool startSession();
    void endSession();
    QSerialPort* getDevice();
    void read(FrameIn* currentFrameIn);
    void write(QByteArray* data);
    void setDevice(QSerialPort* dev);
    QList<QSerialPortInfo> getDeviceList();

signals:
    void refreshFrameIn();

private:
    QList<QSerialPortInfo> _deviceList;
    QTimer _timer;
    QSerialPort *_currentSerialPort = NULL;
    QByteArray _readData;
    FrameIn* _currentFrameIn;

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

  };

#endif // USBDEVICECONTROLLER_H
