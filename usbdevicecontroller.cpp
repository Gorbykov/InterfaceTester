#include "usbdevicecontroller.h"
#include <QMessageBox>

UsbDeviceController::UsbDeviceController()
{
    startSession();
}

UsbDeviceController::~UsbDeviceController()
{
    endSession();
}

int UsbDeviceController::startSession()
{
    _deviceList = QSerialPortInfo::availablePorts();
    return 0;
}

QList<QSerialPortInfo> UsbDeviceController::getDeviceList()
{
    return _deviceList;
}

void UsbDeviceController::endSession()
{
}
