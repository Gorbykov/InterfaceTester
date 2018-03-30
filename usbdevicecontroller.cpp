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
    int r;
    r = libusb_init(&_context);
    if (r<0)
    {
        QMessageBox::information(0, "Ошибка USB", "Ошибка инициализации USB. \nCode: "+ QString::number(r));
        return r;
    }
    libusb_set_debug(_context, 3);
    libusb_device** deviceList;
    int deviceN;
    deviceN = libusb_get_device_list(_context, &deviceList);
    if(deviceN < 0)
    {
        QMessageBox::information(0, "Ошибка USB", "Ошибка устройства USB. \nCode: "+ QString::number(deviceN));
        return r;
    }
    for (int i = 0; i < deviceN; i++)
    {
        _deviceList.push_back(deviceList[i]);
    }
    return r;
}

QVector<libusb_device*> UsbDeviceController::getDeviceList()
{
    return _deviceList;
}

void UsbDeviceController::endSession()
{
    libusb_exit(_context);
}
