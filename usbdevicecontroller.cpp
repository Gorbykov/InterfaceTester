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

int UsbDeviceController::startSession()
{
   //connect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);
    if (_currentSerialPort!=nullptr)
    {
        _currentSerialPort->open(QIODevice::ReadWrite);
    }
    connect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    return _currentSerialPort->isOpen();

}

QList<QSerialPortInfo> UsbDeviceController::getDeviceList()
{
    //disconnect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);

    _deviceList = QSerialPortInfo::availablePorts();
    return _deviceList;
}

void UsbDeviceController::endSession()
{
    disconnect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
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
    _currentFrameIn = currentFrameIn;
    _currentFrameIn->getData().clear();
    _timer.start(TIMEOUT);
}

void UsbDeviceController::handleReadyRead()
{
    _currentFrameIn->getData().append(_currentSerialPort->readAll());
    _textEdit->setText(_currentFrameIn->getData());
    if (!_timer.isActive())
        _timer.start(TIMEOUT);
}

void UsbDeviceController::handleTimeout()
{
    endSession();
}
