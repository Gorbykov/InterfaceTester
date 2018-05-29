#include "usbdevicecontroller.h"
#include <QMessageBox>
#include <QtCore>

UsbDeviceController::UsbDeviceController(QWidget *parent)
{
    connect(this, SIGNAL(refreshFrameIn()), parent, SLOT(refreshFrameIn()));
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
        return _currentSerialPort->open(QIODevice::ReadWrite);
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
    disconnect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    if(_currentSerialPort!=nullptr )
    {
        _currentSerialPort->close();
    }
}

void UsbDeviceController::setDevice(QSerialPort *dev)
{
    _currentSerialPort = dev;
}

QSerialPort* UsbDeviceController::getDevice()
{
    return _currentSerialPort;
}


void UsbDeviceController::write(QByteArray *data)
{
    if(_currentSerialPort!=nullptr || _currentSerialPort->isOpen())
    {
        _currentSerialPort->write(*data);
    }
}

void UsbDeviceController::read(FrameIn *currentFrameIn)
{
    connect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    connect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);
    _currentFrameIn = currentFrameIn;
    _readData.clear();
    _currentFrameIn->setData(_readData);
    _timer.setSingleShot(true);
    _timer.start(TIMEOUT);
}

void UsbDeviceController::handleReadyRead()
{
    _readData.append(_currentSerialPort->readAll());
    _currentFrameIn->setData(_readData);
    emit refreshFrameIn();
    if (!_timer.isActive())
        _timer.start(TIMEOUT);
}

void UsbDeviceController::handleTimeout()
{
    disconnect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    disconnect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);

    //endSession();
}

void UsbDeviceController::handleError(QSerialPort::SerialPortError serialPortError)
{
        _readData.append(_currentSerialPort->errorString());
        _currentFrameIn->setData(_readData);
        emit refreshFrameIn();

}
