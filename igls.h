#ifndef IGLS_H
#define IGLS_H

#include <QObject>
#include <QtNetwork>
#include <QUrlQuery>
#include "common.h"


class IGLS : public QObject
{
    Q_OBJECT
public:
    explicit IGLS(QObject *parent = 0);

signals:
    void Write_DEBUG(QString);
    void write_LOG(int debug_level, QTime timestamp, QString log);
    void AccountInfoUpdate(QString,QString,QString,QString);
    void UpdatePrice(QString,QString);
    void AppendValuetoGraph(quint32,double);
    void NewPosition(QString,double,QDateTime);
    void ConnectedToIG(bool);
    void TransactionListAvailable(QJsonArray);
    void updateTradesInfo();

public slots:
    void GetListOfTrades(int page,QDateTime StartDate ,QDateTime EndDate);
    void GetListOfTradesRequest(QDateTime StartDate,QDateTime EndDate);
    void Connection(StructProxy CurrentProxy, StructAccountInfo CurrentAccount);

private slots:
    void replyFinished(QNetworkReply *);
    void onRequestCompleted();
    void sslErrors(QNetworkReply *, QList<QSslError>);
    void replyencryptedFinished(QNetworkReply *reply);
    void ProcessGetTransactionListRequestAnswer(QNetworkReply *reply);
    void ProcessConnectionRequestAnswer(QNetworkReply *reply);

private:
    QString mainUrl;
    QNetworkProxy proxy;
    QNetworkAccessManager *manager;

    QString ig_username;
    QString ig_password;
    QString ig_API;
    QString ig_AccountType;
    QString AccountID;
    QString URL_LightStreamerEndPoint;
    QString URL_IG;


    QString LS_SesionID ;
    QString LS_ControlAddress;
    QString LS_KeepaliveMillis;
    QString LS_MaxBandwidth;

    QString ls_url;
    QString ls_username;
    QByteArray ls_password;

    QUrl url;


    QString rawHeader_X_SECURITY_TOKEN;
    QString rawHeader_CST;

    quint16 CurrentRequest;

    bool bLightStreamerSubscribeStatus;

    QEventLoop *EventLoopWaitIGConnectAnswer;
    QEventLoop *EventLoopWaitIGGetTrade;


    bool IsConnectedToIG;
    bool IsConnectedToLS;
    bool IsSubscribeToLS;
    bool IsBindToLS;

    bool bGetOpenPositionStatus;
    quint32 tick;

    LiveTransactionStruct CurrentLiveTransaction;

    QBuffer *buffer;

    int CurrentPageNumber;
    int TotalPages;


    QDateTime CurrentStartDate;
    QDateTime CurrentEndDate;


};

#endif // IGLS_H
