#ifndef QTRADING_H
#define QTRADING_H

#include <QMainWindow>
#include <QtNetwork>
#include <QUrlQuery>
#include "QSettings"
#include "common.h"
#include "modeltrades.h"
#include "dialogconnection.h"

#include <QtCharts>

#include "lsclient.h"

using namespace QtCharts;






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


    void Connection_LightStreamerRequest();
    void ProcessConnectionLightStreamerRequestAnswer(QNetworkReply *reply);
private slots:

    void replyFinished(QNetworkReply *);
//    void onProxyAuthenticationRequired(const QNetworkProxy &prox, QAuthenticator *auth);
//    void onauthenticationRequired(QNetworkReply*,QAuthenticator*);

    void Connection_Request();
    void on_pushButton_Get_clicked();

//    void on_comboBox_AccountType_currentIndexChanged(const QString &arg1);

    void createLogFile(QString log_file_path);
    void ProcessGetTransactionListRequestAnswer(QNetworkReply *reply);
    void ProcessConnectionRequestAnswer(QNetworkReply *reply);
    void GetListOfTrades(int page);
    void on_actionConnect_triggered();


private:
    Ui::Qtrading *ui;
    QString mainUrl;
    QNetworkProxy proxy;
    QNetworkAccessManager *manager;

    QString ig_username;
    QString ig_password;
    QString ig_API;
    QString ig_AccountType;
    QString AccountID;
    QString ig_lightstreamerEndPoint;

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

    modelTrades* modeltrades;

    quint16 CurrentRequest;

    int CurrentPageNumber;
    int TotalPages;

    int numberOftradesWon ;
    int numberOftradesLost;
    int numberOftradesFlat ;

    double numberOfPointsWon ;
    double numberOfPointsLost;
    double numberOfPointsTotal;

    DialogConnection *dialogConnection;




};

#endif // QTRADING_H
