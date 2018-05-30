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
    usbDeviceController->startSession();
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
    if  (usbDeviceController == nullptr)
    {
        usbDeviceController = new UsbDeviceController(this);
    }
    usbSettingsWindow = new UsbSettingsWindow(this, usbDeviceController);
    usbSettingsWindow->setWindowTitle("Настройка USB");
    usbSettingsWindow->show();
}

void MainWindow::on_actionEhtTSettings_triggered()
{
    if (ethernetController == nullptr)
    {
        ethernetController = new EthernetController(this);
    }
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

void MainWindow::setTSocket(QUdpSocket *tSocket)
{
    ethernetController->setTSocket(tSocket);
}

void MainWindow::setRSocket(QUdpSocket *rSocket)
{
    ethernetController->setRSocket(rSocket);
}

void MainWindow::on_actionEthStart_triggered()
{

    if (ethernetController->isValid())
    {
        ethernetController->startSession();
    }
    if(currentFrameOut != NULL)
    {
        ethernetController->read(currentFrameIn);
        ethernetController->write(new QByteArray(currentFrameOut->getData()));
        //currentSerialPort->close();
    }
}
