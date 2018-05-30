#ifndef ETHERNETSETTINGWINDOW_H
#define ETHERNETSETTINGWINDOW_H

#include <QDialog>
#include <QAbstractButton>
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
    void setRSocket(QUdpSocket *rSocket);
    void closeSocket();


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::EthernetSettingWindow *ui;
    char _type = 0;
    QUdpSocket* _currentSocket = nullptr;
};

#endif // ETHERNETSETTINGWINDOW_H
