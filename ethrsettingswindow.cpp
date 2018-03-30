#include "ethrsettingswindow.h"
#include "ui_ethrsettingswindow.h"

EthRSettingsWindow::EthRSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EthRSettingsWindow)
{
    ui->setupUi(this);
}

EthRSettingsWindow::~EthRSettingsWindow()
{
    delete ui;
}
