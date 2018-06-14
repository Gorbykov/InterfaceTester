#include "usbdevicecontroller.h"
#include <QMessageBox>
#include <QtCore>

UsbDeviceController::UsbDeviceController(QWidget *parent)
{
    connect(this, SIGNAL(refreshFrameOut()), parent, SLOT(refreshFrameOut()));
    connect(&_timer, &QTimer::timeout, this, &UsbDeviceController::handleTimeout);
    //QObject::connect(this, &UsbDeviceController::refreshFrameIn, )
}

UsbDeviceController::~UsbDeviceController()
{
    endSession();
}


bool UsbDeviceController::startInSession()
{
    return startSession();
}


bool UsbDeviceController::startOutSession()
{
    return startSession();
}

bool UsbDeviceController::startSession()
{
    this->endSession();
    //connect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);
    if (_currentSerialPort!=nullptr)
    {
        _ready = true;
        return _currentSerialPort->open(QIODevice::ReadWrite);
    }
    return false;
}

bool UsbDeviceController::isReady()
{
    return _currentSerialPort!=nullptr && _ready;
}

QList<QSerialPortInfo> UsbDeviceController::getDeviceList()
{
    //disconnect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);

    _deviceList = QSerialPortInfo::availablePorts();
    return _deviceList;
}

void UsbDeviceController::endSession()
{
    if(_currentSerialPort!=nullptr )
    {
        disconnect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
        _currentSerialPort->close();
    }
    _ready == false;
}

void UsbDeviceController::setDevice(QSerialPort *dev)
{
    _currentSerialPort = dev;
}

QSerialPort* UsbDeviceController::getDevice()
{
    return _currentSerialPort;
}


void UsbDeviceController::write(Frame *currentFrame)
{
    if(!isReady())
    {
        return;
    }
    for (int i =0; i<currentFrame->getDelaysIn()->size();i++)
    {
        _writeTiemr.singleShot(currentFrame->getDelaysIn()->at(i),Qt::PreciseTimer,[=]{
            _currentSerialPort->write(*(currentFrame->getDataIn()->at(i)));
        });
    }
}

void UsbDeviceController::read(Frame *currentFrame)
{
    if(!isReady())
    {
        return;
    }
    connect(_currentSerialPort, &QSerialPort::readyRead, this, &UsbDeviceController::handleReadyRead);
    connect(_currentSerialPort, &QSerialPort::errorOccurred, this, &UsbDeviceController::handleError);
    _currentFrame = currentFrame;
    delete _readData;
    delete _readDelays;
    _readDelays = new QVector<int>();
    _currentFrame->setDelaysOut(_readDelays);
    _readData = new QVector<QByteArray*>();
    _currentFrame->setDataOut(_readData);
    _timer.setSingleShot(true);
    _timer.start(TIMEOUT);
}

void UsbDeviceController::handleReadyRead()
{
    _readData->push_back(new QByteArray(_currentSerialPort->readAll()));
    _currentFrame->setDataOut(_readData);
    _readDelays->push_back(TIMEOUT - _timer.remainingTime());
    _currentFrame->setDelaysOut(_readDelays);
    emit refreshFrameOut();
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
//        _readData.append(_currentSerialPort->errorString());
//        _currentFrameIn->setData(_readData);
//        emit refreshFrameIn();

}
