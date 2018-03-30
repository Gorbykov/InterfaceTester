#ifndef USBSETTINGSWINDOW_H
#define USBSETTINGSWINDOW_H

#include <QDialog>
#include "usbdevicecontroller.h"
#include "libusb.h"

namespace Ui {
class UsbSettingsWindow;
}

class UsbSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UsbSettingsWindow(QWidget *parent = 0, UsbDeviceController *usbController = NULL);
    ~UsbSettingsWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::UsbSettingsWindow *ui;
    UsbDeviceController* _usbController=NULL;
};

#endif // USBSETTINGSWINDOW_H
