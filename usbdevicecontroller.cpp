#include "usbdevicecontroller.h"
#include <QMessageBox>
#include <QtCore>

UsbDeviceController::UsbDeviceController(QTextEdit *textEdit)
{
    _textEdit = textEdit;
    connect(&_timer, &QTimer::timeout, this, &UsbDeviceController::handleTimeout);
    //QObject::connect(this, &UsbDeviceController::refreshFrameIn, )
}

UsbDeviceController::~UsbDeviceController()
{
    endSession();
}

bool UsbDeviceController::startSession()
{
   //connect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);
    if (_currentSerialPort!=nullptr)
    {
       return _currentSerialPort->open(QIODevice::WriteOnly);
    }
    return false;
}

QList<QSerialPortInfo> UsbDeviceController::getDeviceList()
{
    //disconnect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);

    _deviceList = QSerialPortInfo::availablePorts();
    return _deviceList;
}

void UsbDeviceController::endSession()
{
    //disconnect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    _currentSerialPort->close();
}

void UsbDeviceController::setDevice(QSerialPort *dev)
{
    _currentSerialPort = dev;
}

QSerialPort* UsbDeviceController::getDevice()
{
    return _currentSerialPort;
}


void UsbDeviceController::write(QByteArray data)
{
    _currentSerialPort->write(data);
}

QByteArray* UsbDeviceController::read(FrameIn *currentFrameIn)
{
    connect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    _currentFrameIn = currentFrameIn;
    _readData.clear();
    _currentFrameIn->getData().clear();
    _timer.start(TIMEOUT);
    return nullptr;
}

void UsbDeviceController::handleReadyRead()
{
    _readData.append(_currentSerialPort->readAll());
    _currentFrameIn->setData(_readData);
    _textEdit->setText(_currentFrameIn->getData());
    if (!_timer.isActive())
        _timer.start(TIMEOUT);
}

void UsbDeviceController::handleTimeout()
{
    endSession();
}
