#ifndef USBDEVICECONTROLLER_H
#define USBDEVICECONTROLLER_H
#include <libusb.h>
#include <QtCore>

class UsbDeviceController
{
public:
    UsbDeviceController();
    ~UsbDeviceController();
    int startSession();
    void endSession();
    QVector<libusb_device*> getDeviceList();


private:
    QVector<libusb_device*> _deviceList;
    libusb_context* _context = NULL;

};

#endif // USBDEVICECONTROLLER_H
