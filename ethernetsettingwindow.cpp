#include "ethernetsettingwindow.h"
#include "ui_ethernetsettingwindow.h"

EthernetSettingWindow::EthernetSettingWindow(QWidget *parent, EthernetController *ethernetController, char type) :
    QDialog(parent),
    ui(new Ui::EthernetSettingWindow)
{
    connect(this, SIGNAL(setTSocket(FullAddress*)),parent,SLOT(setTSocket(FullAddress*)));
    connect(this, SIGNAL(setRSocket(FullAddress*)),parent,SLOT(setRSocket(FullAddress*)));
    connect(this, SIGNAL(closeSocket()),parent,SLOT(closeRSocket()));
    ui->setupUi(this);
    //QUdpSocket* tSocket = ethernetController->getTSocket();
    //QUdpSocket* rSocket = ethernetController->getRSocket();
    FullAddress* tAddress = ethernetController->getTAddress();
    FullAddress* rAddress = ethernetController->getRAddress();

    ui->lineEditPort->setValidator(new QIntValidator(1,65535,this));
    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                      + "\\." + oIpRange
                      + "\\." + oIpRange
                      + "\\." + oIpRange + "$");
    ui->lineEditIp->setValidator(new QRegExpValidator(oIpRegex));
    //QUdpSocket *socket = nullptr;
    FullAddress *address = nullptr;
    _type = type;
    if (_type == 'r')
    {
        //socket = rSocket;
        address = rAddress;
    }
    if (_type == 't')
    {
        //socket = tSocket;
        address = tAddress;
    }

    if (address!=nullptr)
    {
        ui->lineEditIp->setText(address->ip.toString());
        ui->lineEditPort->setText(QString::number(address->port));
       // _currentSocket = socket;
    }
}

EthernetSettingWindow::~EthernetSettingWindow()
{
    delete ui;
}


void EthernetSettingWindow::on_pushButtonOK_clicked()
{

    QHostAddress ip = QHostAddress(ui->lineEditIp->text());
    int port = ui->lineEditPort->text().toInt();
    //QUdpSocket *socket = new QUdpSocket(parent());
    //if (socket ->bind(ip,port))
    FullAddress *address = new FullAddress(ip,port);
    {
        if (_type == 'r')
        {
            emit setRSocket(address);
            emit close();
        }
        if (_type == 't')
        {
            emit setTSocket(address);
            emit close();
        }
    }
}

void EthernetSettingWindow::on_pushButtonCancel_clicked()
{
    //emit closeSocket();
    emit close();
}
