#ifndef ETHRSETTINGSWINDOW_H
#define ETHRSETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class EthRSettingsWindow;
}

class EthRSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EthRSettingsWindow(QWidget *parent = 0);
    ~EthRSettingsWindow();

private:
    Ui::EthRSettingsWindow *ui;
};

#endif // ETHRSETTINGSWINDOW_H
