#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include "framesettingswindow.h"
#include "usbsettingswindow.h"

#include "frameIn.h"
#include "frameOut.h"
#include "usbdevicecontroller.h"

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
    QSerialPort *currentSerialPort = NULL;
    QString lastViewType = "HEX";

public slots:
    void setFrameOut(Frame *newFrame);
    void setUSB(QSerialPort *newPort);

private slots:
    void on_actionFrameOut_triggered();

    void on_actionSaveAll_triggered();

    void on_actionUsbSettings_triggered();

    void on_actionEhtTSettings_triggered();

    void on_actionEhtRSettings_triggered();

    void on_actionUSBStart_triggered();

private:
    Ui::MainWindow *ui;
    FrameSettingsWindow *frameOutSettings = NULL;
    UsbSettingsWindow *usbSettingsWindow = NULL;
    UsbDeviceController *usbDeviceController = NULL;

};

#endif // MAINWINDOW_H
