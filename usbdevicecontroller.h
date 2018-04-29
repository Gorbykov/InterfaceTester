#ifndef USBDEVICECONTROLLER_H
#define USBDEVICECONTROLLER_H
#include <QtCore>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>


class UsbDeviceController
{
public:
    UsbDeviceController();
    ~UsbDeviceController();
    int startSession();
    void endSession();
    QList<QSerialPortInfo> getDeviceList();


private:
    QList<QSerialPortInfo> _deviceList;
};

#endif // USBDEVICECONTROLLER_H
