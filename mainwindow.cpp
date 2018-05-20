#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framesettingswindow.h"
#include "usbsettingswindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionFrameOut_triggered()
{
    frameOutSettings = new FrameSettingsWindow(this,currentFrameOut);
    frameOutSettings->setWindowTitle("Настройка выходного кадра");
    frameOutSettings->show();
}

void MainWindow::setFrameOut(Frame *newFrame)
{
    FrameOut *currFrame = new FrameOut(newFrame->getFrameName(), newFrame->getFileName(), newFrame->getSize(), newFrame->getDelay());
    framesOut.value(newFrame->getFrameName(),currFrame);
    currentFrameOut = currFrame;
    ui->textEditOut->setText(currentFrameOut->getData().toHex(' '));
}

void::MainWindow::setUSB(QSerialPort *newPort)
{
    usbDeviceController->setDevice(newPort);
    usbDeviceController->startSession();
}

void::MainWindow::closeUSB()
{
    usbDeviceController->endSession();
}

void MainWindow::on_actionSaveAll_triggered()
{
    QString viewType = ui->charView->currentText();

    if (currentFrameOut == NULL)
    {
        return;
    }
    QString inStr = ui->textEditOut->toPlainText();

    if (lastViewType == "HEX")
    {
        inStr.remove(QRegExp("[^{0-9a-fA-F}]*"));
        if (inStr.length()%2 == 1)
        {
            QChar lastChar = inStr.back();
            inStr.back() = '0';
            inStr.append(lastChar);
        }
        currentFrameOut->setData(QByteArray::fromHex(inStr.toUtf8()));
        currentFrameOut->saveFile();
    }
    else if(lastViewType == "ASCII")
    {
        currentFrameOut->setData(inStr.toUtf8());
        currentFrameOut->saveFile();
    }
    if(viewType=="HEX")
    {
        ui->textEditOut->setText(currentFrameOut->getData().toHex(' '));
    }
    else if(viewType=="ASCII")
    {
        ui->textEditOut->setText(currentFrameOut->getData());
    }
    lastViewType = viewType;

}


void MainWindow::on_actionUsbSettings_triggered()
{
    if  (usbDeviceController == NULL)
    {
        usbDeviceController = new UsbDeviceController(this->ui->textEditOut);
    }
    usbSettingsWindow = new UsbSettingsWindow(this, usbDeviceController);
    usbSettingsWindow->setWindowTitle("Настройка USB");
    usbSettingsWindow->show();
}

void MainWindow::on_actionEhtTSettings_triggered()
{

}

void MainWindow::on_actionEhtRSettings_triggered()
{
}

void MainWindow::on_actionUSBStart_triggered()
{
    if(currentFrameOut != NULL)
    {
        usbDeviceController->write(currentFrameOut->getData());
        //usbDeviceController->read(currentFrameIn);
        //currentSerialPort->close();
    }
}
