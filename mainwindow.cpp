#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framesettingswindow.h"
#include "usbsettingswindow.h"

#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    usbDeviceController = new UsbDeviceController(this);
    ethernetController = new EthernetController(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setFrameOut(FrameOut *newFrame)
{
    FrameOut *currFrame = newFrame;
    framesOut.value(newFrame->getFrameName(),currFrame);
    currentFrameOut = currFrame;
    printToTextEdit(new QByteArray(currFrame->getData()),ui->textEditOut);
}

void MainWindow::setFrameIn(FrameIn *newFrame)
{
    FrameIn *currFrame = newFrame;
    framesIn.value(newFrame->getFrameName(),currFrame);
    currentFrameIn = currFrame;
    printToTextEdit(new QByteArray(currFrame->getData()),ui->textEditIn);

}
void::MainWindow::setUSB(QSerialPort *newPort)
{
    usbDeviceController->setDevice(newPort);
}

void::MainWindow::closeUSB()
{
    usbDeviceController->endSession();
}


void MainWindow::printToTextEdit(QByteArray* text, QTextEdit *textEdit)
{
    QString viewType = ui->charView->currentText();

    if (currentFrameOut == NULL)
    {
        return;
    }

    if(viewType=="HEX")
    {
        textEdit->setText(text->toHex(' '));
    }
    else if(viewType=="ASCII")
    {
        textEdit->setText(*text);
    }
}

QByteArray* MainWindow::scanFromTextEdit(QTextEdit *textEdit)
{
    QByteArray* text;
    QString inStr = textEdit->toPlainText();
    if (lastViewType == "HEX")
    {
        inStr.remove(QRegExp("[^{0-9a-fA-F}]*"));
        if (inStr.length()%2 == 1)
        {
            QChar lastChar = inStr.back();
            inStr.back() = '0';
            inStr.append(lastChar);
        }
        text = new QByteArray( QByteArray::fromHex(inStr.toUtf8()));
    }
    else if(lastViewType == "ASCII")
    {
        text = new QByteArray(inStr.toUtf8());
    }
    return text;
}

void MainWindow::on_actionSaveAll_triggered()
{
    QByteArray *text = scanFromTextEdit(ui->textEditOut);
    printToTextEdit(text,ui->textEditOut);
    currentFrameOut->setData(*text);
    currentFrameOut->saveFile();
    text = scanFromTextEdit(ui->textEditIn);
    printToTextEdit(text,ui->textEditIn);
    currentFrameIn->setData(*text);
    currentFrameIn->saveFile();
    refreshFrameIn();
}


void MainWindow::on_actionUsbSettings_triggered()
{
    usbSettingsWindow = new UsbSettingsWindow(this, usbDeviceController);
    usbSettingsWindow->setWindowTitle("Настройка USB");
    usbSettingsWindow->show();
}

void MainWindow::on_actionEhtTSettings_triggered()
{
    ethernetSettingWindow = new EthernetSettingWindow(this,ethernetController, 't');
    ethernetSettingWindow->setWindowTitle("Настройка Ethernet передатчика");
    ethernetSettingWindow->show();
}

void MainWindow::on_actionEhtRSettings_triggered()
{
    if (ethernetController == nullptr)
    {
        ethernetController = new EthernetController(this);
    }
    ethernetSettingWindow = new EthernetSettingWindow(this,ethernetController, 'r');
    ethernetSettingWindow->setWindowTitle("Настройка Ethernet преемника");
    ethernetSettingWindow->show();
}

void MainWindow::on_actionUSBStart_triggered()
{
    if(currentFrameOut != NULL)
    {
        usbDeviceController->write(new QByteArray(currentFrameOut->getData()));
        usbDeviceController->read(currentFrameIn);
        //currentSerialPort->close();
    }
}

void MainWindow::on_actionFrame_triggered()
{
    frameOutSettings = new FrameSettingsWindow(this,currentFrameIn,currentFrameOut);
    frameOutSettings->setWindowTitle("Настройка кадров");
    frameOutSettings->show();
}

void MainWindow::on_charView_currentIndexChanged(const QString &arg1)
{
    printToTextEdit(scanFromTextEdit(ui->textEditOut),ui->textEditOut);
    //printToTextEdit(scanFromTextEdit(ui->textEditCompere),ui->textEditCompere);
    printToTextEdit(scanFromTextEdit(ui->textEditIn),ui->textEditIn);
    refreshFrameIn();
    lastViewType = arg1;
}

void MainWindow::refreshFrameIn()
{
    printToTextEdit(new QByteArray(currentFrameIn->getData()),ui->textEditIn);
    printToTextEdit(diff(new QByteArray(currentFrameOut->getData()),new QByteArray(currentFrameIn->getData())),ui->textEditCompere);
}


QByteArray *MainWindow::diff(QByteArray *A, QByteArray *B)
{
    int max = qMax(A->size(),B->size());
    int min = qMin(A->size(),B->size());
    QByteArray* M = A->size()==max?A:B;
    QByteArray* C = new QByteArray( max, (char) 0xff) ;
    for (int i = 0 ; i< max ; i++)
    {
        if(i<min)
        {
            (*C)[i] =(A->at(i) ^ B->at(i));
        }
        else
        {
            (*C)[i] =(M->at(i) ^ 0x00);
        }

    }
    return C;
}

void MainWindow::closeSocket()
{
    ethernetController->endSession();
}

void MainWindow::setTSocket(FullAddress *tAddress)
{
    ethernetController->setTSocket(tAddress);
}

void MainWindow::setRSocket(FullAddress *rAddress)
{
    ethernetController->setRSocket(rAddress);
}

void MainWindow::on_actionEthStart_triggered()
{
    if(currentFrameOut != NULL)
    {
        ethernetController->read(currentFrameIn);
        ethernetController->write(new QByteArray(currentFrameOut->getData()));
        //currentSerialPort->close();
    }
}

void MainWindow::on_actionStart_triggered()
{
    int inSelect=-1;
    int outSelect=-1;

    if(ui->radioButtonUsbIn->isChecked())
    {
        inSelect = 0;
    }
    if(ui->radioButtonEthIn->isChecked())
    {
        inSelect = 1;
    }
    if(ui->radioButtonUsbOut->isChecked())
    {
        outSelect = 0;
    }
    if(ui->radioButtonEthOut->isChecked())
    {
        outSelect = 1;
    }

    if(currentFrameOut == NULL)
    {
        return;
    }
    if(inSelect+outSelect != -2)
    {
        switch (outSelect) {
        case 0:
            usbDeviceController->read(currentFrameIn);
            break;
        case 1:
            ethernetController->read(currentFrameIn);
            break;
        }
        switch (inSelect) {
        case 0:
            usbDeviceController->write(new QByteArray(currentFrameOut->getData()));
            break;
        case 1:
            ethernetController->write(new QByteArray(currentFrameOut->getData()));
        }
    }

}

void MainWindow::on_pushButtonIn_clicked()
{
    int inSelect=-1;
    if(ui->radioButtonUsbIn->isChecked())
    {
        inSelect = 0;
    }
    if(ui->radioButtonEthIn->isChecked())
    {
        inSelect = 1;
    }

    switch (inSelect) {
    case 0:
        if(currentFrameOut != NULL)
        {
            usbDeviceController->startInSession();
        }
        break;
    case 1:
        if(currentFrameOut != NULL)
        {
            ethernetController->startInSession();
        }
        break;
    }
}

void MainWindow::on_pushButtonOut_clicked()
{
    int outSelect=-1;
    if(ui->radioButtonUsbOut->isChecked())
    {
        outSelect = 0;
    }
    if(ui->radioButtonEthOut->isChecked())
    {
        outSelect = 1;
    }

    switch (outSelect) {
    case 0:
        if(currentFrameOut != NULL)
        {
            usbDeviceController->startOutSession();
        }
        break;
    case 1:
        if(currentFrameOut != NULL)
        {
            ethernetController->startOutSession();
        }
        break;
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    on_actionStart_triggered();
}

void MainWindow::on_pushButtonEnd_clicked()
{
    usbDeviceController->endSession();
    ethernetController->endSession();
}
