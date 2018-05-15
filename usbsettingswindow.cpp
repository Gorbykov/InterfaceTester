#include "usbsettingswindow.h"
#include "ui_usbsettingswindow.h"
#include <QMessageBox>
#include <cstring>

UsbSettingsWindow::UsbSettingsWindow(QWidget *parent, UsbDeviceController *usbController) :
    QDialog(parent),
    ui(new Ui::UsbSettingsWindow)
{

    connect(this, SIGNAL(setUSB(QSerialPort*)),parent,SLOT(setUSB(QSerialPort*)));
    ui->setupUi(this);
    QComboBox *comboBox = ui->comboBoxDevicecList;
    comboBox->clear();
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
    QSerialPort *newPort = new QSerialPort(ui->comboBoxDevicecList->currentText());
    newPort->setBaudRate(QSerialPort::Baud9600);
    emit setUSB(newPort);
    emit close();
}
