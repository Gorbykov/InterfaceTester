#include "ethernetsettingwindow.h"
#include "ui_ethernetsettingwindow.h"

EthernetSettingWindow::EthernetSettingWindow(QWidget *parent, EthernetController *ethernetController, char type) :
    QDialog(parent),
    ui(new Ui::EthernetSettingWindow)
{
    connect(this, SIGNAL(setTSocket(QUdpSocket*)),parent,SLOT(setTSocket(QUdpSocket*)));
    connect(this, SIGNAL(setRSocket(QUdpSocket*)),parent,SLOT(setRSocket(QUdpSocket*)));
    connect(this, SIGNAL(closeTSocket()),parent,SLOT(closeTSocket()));
    connect(this, SIGNAL(closeRSocket()),parent,SLOT(closeRSocket()));
    ui->setupUi(this);
    QUdpSocket* tSocket = ethernetController->getTSocket();
    QUdpSocket* rSocket = ethernetController->getRSocket();
    ui->lineEditPort->setValidator(new QIntValidator(1,65535,this));
    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange + "$");
    ui->lineEditIp->setValidator(new QRegExpValidator(oIpRegex));
    QUdpSocket *socket = nullptr;
    _type = type;
    if (_type == 'r')
    {
        socket = rSocket;
    }
    if (_type == 't')
    {
        socket = rSocket;
    }

    if (socket!=nullptr)
    {
        ui->lineEditIp->setText(socket->peerAddress().toString());
        ui->lineEditPort->setText(QString::number(socket->peerPort()));
    }
}

EthernetSettingWindow::~EthernetSettingWindow()
{
    delete ui;
}

void EthernetSettingWindow::on_buttonBox_accepted()
{
    QHostAddress ip = QHostAddress(ui->lineEditIp->text());
    qint16 port = ui->lineEditPort->text().toInt();
    QUdpSocket *socket = new QUdpSocket();
    socket ->bind(ip,port);
    if (_type == 'r')
    {
        emit setRSocket(socket);
    }
    if (_type == 't')
    {
        emit setTSocket(socket);
    }
}
