#ifndef ETHERNETSETTINGWINDOW_H
#define ETHERNETSETTINGWINDOW_H

#include <QDialog>

namespace Ui {
class EthernetSettingWindow;
}

class EthernetSettingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EthernetSettingWindow(QWidget *parent = 0);
    ~EthernetSettingWindow();

private:
    Ui::EthernetSettingWindow *ui;
};

#endif // ETHERNETSETTINGWINDOW_H
