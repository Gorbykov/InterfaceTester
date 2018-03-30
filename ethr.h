#ifndef ETHR_H
#define ETHR_H


#include <QDialog>
#include <QString>
#include <QVector>
#include <QStatusBar>

class QLabel;
class QTcpServer;
class QNetworkSession;

class EthR: public QDialog
{
    Q_OBJECT
public:
    explicit EthR(QWidget *parent = nullptr);

private slots:
    void sessionOpened();
    void sendFortune();

private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;
};

#endif // ETHR_H
