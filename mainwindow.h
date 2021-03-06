#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include "framesettingswindow.h"
#include "ethernetsettingwindow.h"

#include "frame.h"
#include "ethernetcontroller.h"
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void setFrame(Frame *newFrame);

    void setTSocket(FullAddress *tPCAddress, FullAddress *rDevAddress);
    void setRSocket(FullAddress *tDevAddress, FullAddress *rPCAddress);
    void closeSocket();

    void refreshFrameOut();
    void refreshSendingStatus(bool status);

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


    void on_actionSaveIn_triggered();

    void on_actionSaveOut_triggered();

    void on_actionOpenIn_triggered();

    void on_actionOpenOut_triggered();

    void on_actionOpenAll_triggered();

private:
    Frame *_frame = nullptr;
    QString _lastViewType = "HEX";
    Ui::MainWindow *ui;
    FrameSettingsWindow *frameSettings = nullptr;
    EthernetController *ethernetController = nullptr;
    EthernetSettingWindow *ethernetSettingWindow = nullptr;

    void printDelays(QVector<int>* delays, QVector<int> *sizes,  QTextEdit *textEdit);
    void printToTextEdit(QVector<QByteArray*> *data, QTextEdit *textEdit);
    void printToTextEdit(QByteArray *text, QTextEdit *textEdit);
    QByteArray* scanFromTextEdit(QTextEdit *textEdit);
    QVector<int>* diff(QVector<int>* A, QVector<int>* B);
    QByteArray* diff(QByteArray* A, QByteArray* B);
    QVector<QByteArray*>* diff(QVector<QByteArray *> *A, QVector<QByteArray *> *B);
    QByteArray* validateFrame(QString* inStr);
    QByteArray* validateDelays(QString* inStr);


};

#endif // MAINWINDOW_H
