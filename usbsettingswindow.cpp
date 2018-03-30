#include "usbsettingswindow.h"
#include "ui_usbsettingswindow.h"
#include <QMessageBox>
#include <cstring>

UsbSettingsWindow::UsbSettingsWindow(QWidget *parent, UsbDeviceController *usbController) :
    QDialog(parent),
    ui(new Ui::UsbSettingsWindow)
{
    _usbController = usbController;
    QVector<libusb_device*> deviceList;
    deviceList = usbController->getDeviceList();
    ui->setupUi(this);
    QComboBox *comboBox = ui->comboBoxDevicecList;
    for (QVector<libusb_device*>::iterator i = deviceList.begin(); i != deviceList.end(); i++)
    {
        libusb_device_descriptor descriptor;
        int r = libusb_get_device_descriptor(*i, &descriptor);
        if(r < 0)
        {
            QMessageBox::information(0, "Ошибка USB", "Ошибка получения информации об устройстве USB. \nCode: "+ QString::number(r));
        }
        char str1[256];
        unsigned char str2[256];
        libusb_device_handle* handle = NULL;
        r = libusb_open(*i,&handle);

        if(r == 0)
        {
            r = libusb_get_string_descriptor_ascii(handle,descriptor.iProduct, str2, 256);
            strcpy(str1, (char*)str2);
            comboBox->addItem(QString(str1));
        }
        else
        {
            QMessageBox::information(0, "Ошибка USB", "Ошибка открытия устройства USB. \nCode: "+ QString::number(r));
        }
        libusb_close(handle);
    }
}

UsbSettingsWindow::~UsbSettingsWindow()
{
    delete ui;
}

void UsbSettingsWindow::on_buttonBox_accepted()
{

}
