#include "ethernetsettingwindow.h"
#include "ui_ethernetsettingwindow.h"

EthernetSettingWindow::EthernetSettingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EthernetSettingWindow)
{
    ui->setupUi(this);
}

EthernetSettingWindow::~EthernetSettingWindow()
{
    delete ui;
}
