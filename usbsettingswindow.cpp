#include "usbsettingswindow.h"
#include "ui_usbsettingswindow.h"
#include <QMessageBox>
#include <cstring>

UsbSettingsWindow::UsbSettingsWindow(QWidget *parent, UsbDeviceController *usbController) :
    QDialog(parent),
    ui(new Ui::UsbSettingsWindow)
{
    ui->setupUi(this);
    QComboBox *comboBox = ui->comboBoxDevicecList;
    for (const QSerialPortInfo &dev : usbController->getDeviceList()) {
           comboBox->addItem(dev.portName());
        }
}

UsbSettingsWindow::~UsbSettingsWindow()
{
    delete ui;
}

void UsbSettingsWindow::on_buttonBox_accepted()
{

}
