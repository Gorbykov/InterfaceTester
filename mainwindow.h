#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include "framesettingswindow.h"
#include "usbsettingswindow.h"
#include "ethernetsettingwindow.h"

#include "frameIn.h"
#include "frameOut.h"
#include "usbdevicecontroller.h"
#include "ethernetcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMap<QString, FrameOut*> framesOut;
    QMap<QString, FrameIn*> framesIn;
    FrameOut *currentFrameOut = NULL;
    FrameIn *currentFrameIn = NULL;
    QString lastViewType = "HEX";


public slots:
    void setFrameOut(FrameOut *newFrame);
    void setFrameIn(FrameIn *newFrame);
    void setUSB(QSerialPort *newPort);

    void setTSocket(FullAddress *tAddress);
    void setRSocket( FullAddress *rAddress);
    void closeSocket();

    void closeUSB();
    void refreshFrameIn();

private slots:

    void on_actionSaveAll_triggered();

    void on_actionUsbSettings_triggered();

    void on_actionEhtTSettings_triggered();

    void on_actionEhtRSettings_triggered();

    void on_actionUSBStart_triggered();

    void on_actionFrame_triggered();

    void on_charView_currentIndexChanged(const QString &arg1);

    void on_actionEthStart_triggered();

    void on_actionStart_triggered();

    void on_pushButtonIn_clicked();

    void on_pushButtonStart_clicked();

    void on_pushButtonEnd_clicked();

    void on_pushButtonOut_clicked();

private:
    Ui::MainWindow *ui;
    FrameSettingsWindow *frameOutSettings = nullptr;
    UsbSettingsWindow *usbSettingsWindow = nullptr;
    UsbDeviceController *usbDeviceController = nullptr;
    EthernetController *ethernetController = nullptr;
    EthernetSettingWindow *ethernetSettingWindow = nullptr;

    void printToTextEdit(QByteArray *text, QTextEdit *textEdit);
    QByteArray* scanFromTextEdit(QTextEdit *textEdit);
    QByteArray* diff(QByteArray* A, QByteArray* B);

};

#endif // MAINWINDOW_H
