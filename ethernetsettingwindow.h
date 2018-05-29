#ifndef ETHERNETSETTINGWINDOW_H
#define ETHERNETSETTINGWINDOW_H

#include <QDialog>
#include "ethernetcontroller.h"

namespace Ui {
class EthernetSettingWindow;
}

class EthernetSettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EthernetSettingWindow(QWidget *parent = 0, EthernetController* ethernetController = nullptr, char type=0);
    ~EthernetSettingWindow();
private:
signals:
    void setTSocket(QUdpSocket *tSocket);
    void closeTSocket();
    void setRSocket(QUdpSocket *rSocket);
    void closeRSocket();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::EthernetSettingWindow *ui;
    char _type = 0;
};

#endif // ETHERNETSETTINGWINDOW_H
