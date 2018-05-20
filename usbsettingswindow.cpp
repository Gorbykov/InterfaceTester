#include "usbsettingswindow.h"
#include "ui_usbsettingswindow.h"
#include <QMessageBox>

UsbSettingsWindow::UsbSettingsWindow(QWidget *parent, UsbDeviceController *usbController) :
    QDialog(parent),
    ui(new Ui::UsbSettingsWindow)
{

    connect(this, SIGNAL(setUSB(QSerialPort*)),parent,SLOT(setUSB(QSerialPort*)));
    connect(this, SIGNAL(closeUSB()),parent,SLOT(closeUSB()));
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
    emit closeUSB();
    emit setUSB(newPort);
    //emit close();
}

void UsbSettingsWindow::on_buttonBox_rejected()
{
    emit closeUSB();
    //emit close();
}
