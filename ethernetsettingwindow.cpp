#include "ethernetsettingwindow.h"
#include "ui_ethernetsettingwindow.h"

EthernetSettingWindow::EthernetSettingWindow(QWidget *parent, EthernetController *ethernetController, char type) :
    QDialog(parent),
    ui(new Ui::EthernetSettingWindow)
{
    connect(this, SIGNAL(setTSocket(FullAddress*,FullAddress*)),parent,SLOT(setTSocket(FullAddress*,FullAddress*)));
    connect(this, SIGNAL(setRSocket(FullAddress*,FullAddress*)),parent,SLOT(setRSocket(FullAddress*,FullAddress*)));
    connect(this, SIGNAL(closeSocket()),parent,SLOT(closeRSocket()));
    ui->setupUi(this);
    FullAddress* tPCAddress = ethernetController->getTPCAddress();
    FullAddress* rPCAddress = ethernetController->getRPCAddress();
    FullAddress* tDevAddress = ethernetController->getTDevAddress();
    FullAddress* rDevAddress = ethernetController->getRDevAddress();

    ui->lineEditPortR ->setValidator(new QIntValidator(1,65535,this));
    ui->lineEditPortT ->setValidator(new QIntValidator(1,65535,this));
    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                      + "\\." + oIpRange
                      + "\\." + oIpRange
                      + "\\." + oIpRange + "$");
    ui->lineEditIpT->setValidator(new QRegExpValidator(oIpRegex));
    ui->lineEditIpR->setValidator(new QRegExpValidator(oIpRegex));
    FullAddress *rAddress = nullptr;
    FullAddress *tAddress = nullptr;
    _type = type;
    if (_type == 'r')
    {
        ui->labelT->setText("Выход устройства");
        ui->labelR->setText("Вход компьютера");
        rAddress = rPCAddress;
        tAddress = tDevAddress;
    }
    if (_type == 't')
    {
        ui->labelT->setText("Выход компьютера");
        ui->labelR->setText("Вход устройства");
        rAddress = rDevAddress;
        tAddress = tPCAddress;
    }

    if (rAddress!=nullptr && tAddress!=nullptr)
    {
        ui->lineEditIpT->setText(tAddress->ip.toString());
        ui->lineEditPortT->setText(QString::number(tAddress->port));
        ui->lineEditIpR->setText(rAddress->ip.toString());
        ui->lineEditPortR->setText(QString::number(rAddress->port));
    }

    if (_type == 'r')
    {
        ui->lineEditIpT->setEnabled(false);
        ui->lineEditPortT->setEnabled(false);
    }
}

EthernetSettingWindow::~EthernetSettingWindow()
{
    delete ui;
}


void EthernetSettingWindow::on_pushButtonOK_clicked()
{

    QHostAddress ip = QHostAddress(ui->lineEditIpT->text());
    int port = ui->lineEditPortT->text().toInt();
    FullAddress *tAddress = new FullAddress(ip,port);

    ip = QHostAddress(ui->lineEditIpR->text());
    port = ui->lineEditPortR->text().toInt();
    FullAddress *rAddress = new FullAddress(ip,port);
    {
        if (_type == 'r')
        {
            emit setRSocket(tAddress,rAddress);
            emit close();
        }
        if (_type == 't')
        {
            emit setTSocket(tAddress,rAddress);
            emit close();
        }
    }
}

void EthernetSettingWindow::on_pushButtonCancel_clicked()
{
    emit close();
}
