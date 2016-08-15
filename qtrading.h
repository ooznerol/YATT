#ifndef QTRADING_H
#define QTRADING_H

#include <QMainWindow>
#include <QtNetwork>
#include <QUrlQuery>
#include "QSettings"
#include "common.h"
#include "modeltrades.h"

#include <QtCharts>

using namespace QtCharts;

#define LOG_DEBUG_FULL      0x01
#define LOG_DEBUG           0x02
#define LOG_NORMAL          0x04
#define LOG_WARNING         0x08
#define LOG_ERROR           0x10
#define LOG_SUCCESS         0x20




namespace Ui {
class Qtrading;
}

class Qtrading : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qtrading(QWidget *parent = 0);
    ~Qtrading();



public slots:
    void sslErrors(QNetworkReply *, QList<QSslError>);
    void replyencryptedFinished(QNetworkReply *reply);
    void write_LOG(int debug_level, QTime timestamp, QString log);
private slots:

    void replyFinished(QNetworkReply *);
//    void onProxyAuthenticationRequired(const QNetworkProxy &prox, QAuthenticator *auth);
//    void onauthenticationRequired(QNetworkReply*,QAuthenticator*);

    void on_pushButton_Connect_clicked();
    void on_pushButton_Get_clicked();

    void on_comboBox_AccountType_currentIndexChanged(const QString &arg1);

    void createLogFile(QString log_file_path);
private:
    Ui::Qtrading *ui;
    QString mainUrl;
    QNetworkProxy proxy;
    QNetworkAccessManager *manager;

    QString ig_username;
    QString ig_password;
    QString ig_API;
    QString ig_AccountType;

    QString ls_url;
    QString ls_username;
    QByteArray ls_password;

    QSettings *GUI_Settings;

    QUrl url;


    QString rawHeader_X_SECURITY_TOKEN;
    QString rawHeader_CST;

    QList <TransactionStruct> ListOfTransaction;

    QFile log_file;

    QChart *chart;



};

#endif // QTRADING_H
