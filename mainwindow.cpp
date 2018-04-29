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


void MainWindow::on_actionSaveAll_triggered()
{
    if (currentFrameOut == NULL)
    {
        return;
    }
    QString inStr = ui->textEditOut->toPlainText();
    inStr.remove(QRegExp("[^{0-9a-fA-F}]*"));
    if (inStr.length()%2 == 1)
    {
        QChar lastChar = inStr.back();
        inStr.back() = '0';
        inStr.append(lastChar);
    }
    currentFrameOut->setData(QByteArray::fromHex(inStr.toUtf8()));
    currentFrameOut->saveFile();    
    ui->textEditOut->setText(currentFrameOut->getData().toHex(' '));
}


void MainWindow::on_actionUsbSettings_triggered()
{
    if  (usbDeviceController == NULL)
    {
        usbDeviceController = new UsbDeviceController();
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
