#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framesettingswindow.h"

#include <QMessageBox>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //usbDeviceController = new UsbDeviceController(this);
    ethernetController = new EthernetController(this);
    connect(ethernetController, SIGNAL(refreshSendingStatus(bool)),this,SLOT(refreshSendingStatus(bool)) );
    ui->lineEditTimeout->setValidator(new QIntValidator(this));
    ui->lineEditTimeout->setText("10000");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setFrame(Frame *newFrame)
{
    Frame *currFrame = newFrame;
    _frame = currFrame;
    if(!_frame->openAll())
    {
        QMessageBox::warning(0,"Ошибка открытия файлов","Невозможно открыть файлы");
        delete _frame;
        _frame = nullptr;
        return;
    }
    printToTextEdit(_frame->getFullDataIn(),ui->textEditIn);
    printDelays(_frame->getFullDelaysIn(),_frame->getFullSizesIn(),ui->textEditInDelays);
    //printToTextEdit(_frame->getDataOut(),ui->textEditOut);
    refreshFrameOut();
}

//void::MainWindow::setUSB(QSerialPort *newPort)
//{
//    //usbDeviceController->setDevice(newPort);
//}

//void::MainWindow::closeUSB()
//{
//   // usbDeviceController->endSession();
//}


void MainWindow::printDelays(QVector<int>* delays, QVector<int>* sizes,  QTextEdit *textEdit)
{
    textEdit->clear();
    for(int i = 0; i < delays->size(); i++ )
    {
        textEdit->append(QString::number(sizes->at(i)) + " " + QString::number(delays->at(i)) );
    }
}

void MainWindow::printToTextEdit(QVector<QByteArray*> *data, QTextEdit *textEdit)
{
    QByteArray *text = new QByteArray();
    foreach (auto s, *data) {
        text->append(*s);
    }
    printToTextEdit(text,textEdit);
}

void MainWindow::printToTextEdit(QByteArray *text, QTextEdit *textEdit)
{
    QString viewType = ui->charView->currentText();

    if (_frame == nullptr)
    {
        return;
    }

    if(viewType=="HEX")
    {
        textEdit->setText(text->toHex(' '));
    }
    else if(viewType=="UTF-8")
    {
        textEdit->setText(*text);
    }
}

QByteArray* MainWindow::scanFromTextEdit(QTextEdit *textEdit)
{
    QString* inStr = new QString(textEdit->toPlainText());
    return validateFrame(inStr);
}



void MainWindow::on_actionUsbSettings_triggered()
{
    //    usbSettingsWindow = new UsbSettingsWindow(this, usbDeviceController);
    //    usbSettingsWindow->setWindowTitle("Настройка USB");
    //    usbSettingsWindow->show();
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
    if(_frame != nullptr)
    {
        //usbDeviceController->write(_frame);
        //usbDeviceController->read(_frame);
        //currentSerialPort->close();
    }
}

void MainWindow::on_actionFrame_triggered()
{
    frameSettings = new FrameSettingsWindow(this,_frame);
    frameSettings->setWindowTitle("Настройка кадров");
    frameSettings->show();
}

void MainWindow::on_charView_currentIndexChanged(const QString &arg1)
{
    //printToTextEdit(scanFromTextEdit(ui->textEditOut),ui->textEditOut);
    //printToTextEdit(scanFromTextEdit(ui->textEditCompere),ui->textEditCompere);
    printToTextEdit(scanFromTextEdit(ui->textEditIn),ui->textEditIn);
    refreshFrameOut();
    _lastViewType = arg1;
}

void MainWindow::refreshFrameOut()
{
    if(_frame == nullptr)
    {
        return;
    }
    printToTextEdit(_frame->getDataOut(),ui->textEditOut);
    printDelays(_frame->getDelaysOut(),_frame->getSizesOut(),ui->textEditOutDelays);
    printToTextEdit(diff(_frame->getDataOut(),_frame->getDataIn()),ui->textEditCompere);
    printDelays(diff(_frame->getDelaysOut(),_frame->getDelaysIn()),diff(_frame->getSizesOut(),_frame->getSizesIn()),ui->textEditCompereDelays);
}

void MainWindow::refreshSendingStatus(bool status)
{
    if (status)
    {
        ui->labelSending->setText("On");
    }
    else
    {
        ui->labelSending->setText("Off");
    }
}

QVector<int> *MainWindow::diff(QVector<int> *A, QVector<int> *B)
{
    int max = qMax(A->size(),B->size());
    int min = qMin(A->size(),B->size());
    int sign = 0;
    QVector<int>* M;
    if(A->size()==max)
    {
        M = A;
        sign = 1;
    }
    else
    {
        M = B;
        sign = -1;
    }
    QVector<int>* C = new QVector<int>( max, 0) ;
    for (int i = 0 ; i< max ; i++)
    {
        if(i<min)
        {
            (*C)[i] =(A->at(i) - B->at(i));
        }
        else
        {
            (*C)[i] =M->at(i)*sign;
        }

    }
    return C;
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

QVector<QByteArray*>* MainWindow::diff(QVector<QByteArray *> *A, QVector<QByteArray *> *B)
{
    int max = qMax(A->size(),B->size());
    int min = qMin(A->size(),B->size());
    QVector<QByteArray *>  *M = A->size()==max?A:B;
    QVector<QByteArray *>  *C = new QVector<QByteArray *>(max,nullptr);
    for (int i = 0 ; i< max ; i++)
    {
        if(i<min)
        {
            (*C)[i] =diff(A->at(i),B->at(i));
        }
        else
        {
            (*C)[i] =diff(M->at(i),new QByteArray(M->at(i)->size(), (char)0x00));
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
    if(_frame != nullptr)
    {
        ethernetController->read(_frame);
        ethernetController->write(_frame);
        //currentSerialPort->close();
    }
}

void MainWindow::on_actionStart_triggered()
{
    int inSelect=-1;
    int outSelect=-1;

    if(ui->radioButtonRemoteEthIn->isChecked())
    {
        inSelect = 0;
    }
    if(ui->radioButtonEthIn->isChecked())
    {
        inSelect = 1;
    }
    if(ui->radioButtonRemoteEthOut->isChecked())
    {
        outSelect = 0;
    }
    if(ui->radioButtonEthOut->isChecked())
    {
        outSelect = 1;
    }

    if(_frame == nullptr)
    {
        return;
    }
    if(inSelect+outSelect != -2)
    {
        switch (outSelect) {
        case 0:
            break;
        case 1:
            ethernetController->setTimeout(ui->lineEditTimeout->text().toInt());
            ethernetController->read(_frame);
            break;
        }
        switch (inSelect) {
        case 0:
            break;
        case 1:
            ethernetController->write(_frame);
        }
    }

}

void MainWindow::on_pushButtonIn_clicked()
{
    int inSelect=-1;
    if(ui->radioButtonRemoteEthIn->isChecked())
    {
        inSelect = 0;
    }
    if(ui->radioButtonEthIn->isChecked())
    {
        inSelect = 1;
    }
    bool ok = false;
    switch (inSelect) {
    case 0:
        if(_frame != nullptr)
        {
            ok = true;
        }
        break;
    case 1:
        if(_frame != nullptr)
        {
            ok = ethernetController->startInSession();
        }
        break;
    }

    if (ok)
    {
        ui->labelInOpen->setText("On");
    }
    else
    {
        ui->labelInOpen->setText("Off");
    }
}

void MainWindow::on_pushButtonOut_clicked()
{
    int outSelect=-1;
    if(ui->radioButtonRemoteEthOut->isChecked())
    {
        outSelect = 0;
    }
    if(ui->radioButtonEthOut->isChecked())
    {
        outSelect = 1;
    }

    bool ok = false;
    switch (outSelect) {
    case 0:
        if(_frame != nullptr)
        {
            ok = true;
        }
        break;
    case 1:
        if(_frame != nullptr)
        {
            ok = ethernetController->startOutSession();
        }
        break;
    }
    if (ok)
    {
        ui->labelOutOpen->setText("On");
    }
    else
    {
        ui->labelOutOpen->setText("Off");
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    on_actionStart_triggered();
}

void MainWindow::on_pushButtonEnd_clicked()
{
    //usbDeviceController->endSession();

    ui->labelInOpen->setText("Off");
    ui->labelOutOpen->setText("Off");
    ethernetController->endSession();
}


QByteArray* MainWindow::validateFrame(QString *inStr)
{
    QByteArray *text;
    if (_lastViewType == "HEX")
    {
        inStr->remove(QRegExp("[^{0-9a-fA-F}]*"));
        if (inStr->length()%2 == 1)
        {
            QChar lastChar = inStr->back();
            inStr->back() = '0';
            inStr->append(lastChar);
        }
        text = new QByteArray( QByteArray::fromHex(inStr->toUtf8()));
    }
    else if(_lastViewType == "UTF-8")
    {
        text = new QByteArray(inStr->toUtf8());
    }
    delete inStr;
    return text;
}

QByteArray* MainWindow::validateDelays(QString *inStr)
{
    QByteArray *text;
    QRegExpValidator validator;
    validator.setRegExp(QRegExp("([0-9]+ [0-9]+)\n?"));
    validator.fixup(*inStr);
    text = new QByteArray(inStr->toUtf8());
    delete inStr;
    return text;
}

void MainWindow::on_actionSaveAll_triggered()
{
    if (_frame != nullptr)
    {
        _frame->saveAll();
    }
}

void MainWindow::on_actionSaveIn_triggered()
{
    if (_frame != nullptr)
    {
        _frame->saveIn();
    }
}

void MainWindow::on_actionSaveOut_triggered()
{
    if (_frame != nullptr)
    {
        _frame->saveOut();
    }
}

void MainWindow::on_actionOpenIn_triggered()
{
    if(_frame==nullptr)
    {
        return;
    }

    if(!_frame->openIn())
    {
        QMessageBox::warning(0,"Ошибка открытия файлов","Невозможно открыть файлы");
        delete _frame;
        _frame = nullptr;
        return;
    }
    printToTextEdit(_frame->getFullDataIn(),ui->textEditIn);
    printDelays(_frame->getFullDelaysIn(),_frame->getFullSizesIn(),ui->textEditInDelays);
    refreshFrameOut();
}

void MainWindow::on_actionOpenOut_triggered()
{

    if(_frame==nullptr)
    {
        return;
    }

    if(!_frame->openOut())
    {
        QMessageBox::warning(0,"Ошибка открытия файлов","Невозможно открыть файлы");
        delete _frame;
        _frame = nullptr;
        return;
    }
    printToTextEdit(_frame->getFullDataIn(),ui->textEditIn);
    printDelays(_frame->getFullDelaysIn(),_frame->getFullSizesIn(),ui->textEditInDelays);
    refreshFrameOut();
}

void MainWindow::on_actionOpenAll_triggered()
{

    if(_frame==nullptr)
    {
        return;
    }

    if(!_frame->openAll())
    {
        QMessageBox::warning(0,"Ошибка открытия файлов","Невозможно открыть файлы");
        delete _frame;
        _frame = nullptr;
        return;
    }
    printToTextEdit(_frame->getFullDataIn(),ui->textEditIn);
    printDelays(_frame->getFullDelaysIn(),_frame->getFullSizesIn(),ui->textEditInDelays);
    refreshFrameOut();
}
