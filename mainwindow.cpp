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


void MainWindow::setFrameOut(FrameOut *newFrame)
{
    FrameOut *currFrame = newFrame;
    framesOut.value(newFrame->getFrameName(),currFrame);
    currentFrameOut = currFrame;
    printToTextEdit(currFrame->getData(),ui->textEditOut);
}

void MainWindow::setFrameIn(FrameIn *newFrame)
{
    FrameIn *currFrame = newFrame;
    framesIn.value(newFrame->getFrameName(),currFrame);
    currentFrameIn = currFrame;
    printToTextEdit(currFrame->getData(),ui->textEditIn);
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


void MainWindow::printToTextEdit(QByteArray text, QTextEdit *textEdit)
{
    QString viewType = ui->charView->currentText();

    if (currentFrameOut == NULL)
    {
        return;
    }

    if(viewType=="HEX")
    {
        textEdit->setText(text.toHex(' '));
    }
    else if(viewType=="ASCII")
    {
        ui->textEditOut->setText(text);
    }
    lastViewType = viewType;
}

QByteArray MainWindow::scanFromTextEdit(QTextEdit *textEdit)
{
    QByteArray text;
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
        text = QByteArray::fromHex(inStr.toUtf8());
    }
    else if(lastViewType == "ASCII")
    {
        text = inStr.toUtf8();
    }
    return text;
}

void MainWindow::on_actionSaveAll_triggered()
{
    QByteArray text = scanFromTextEdit(ui->textEditOut);
    printToTextEdit(text,ui->textEditOut);
    currentFrameOut->setData(text);
    currentFrameOut->saveFile();
    text = scanFromTextEdit(ui->textEditIn);
    printToTextEdit(text,ui->textEditIn);
    currentFrameIn->setData(text);
    currentFrameIn->saveFile();

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

void MainWindow::on_actionUSBStart_triggered()
{
    if(currentFrameOut != NULL)
    {
        usbDeviceController->write(currentFrameOut->getData());
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
    printToTextEdit(scanFromTextEdit(ui->textEditCompere),ui->textEditCompere);
    printToTextEdit(scanFromTextEdit(ui->textEditIn),ui->textEditIn);
}

void MainWindow::refreshFrameIn()
{
    printToTextEdit(currentFrameIn->getData(),ui->textEditIn);
}
