#ifndef DIALOGCONNECTION_H
#define DIALOGCONNECTION_H

#include <QDialog>
#include "QSettings"
#include <QtNetwork>
#include <QUrlQuery>
#include "common.h"

namespace Ui {
class DialogConnection;
}

class DialogConnection : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnection(QSettings *settings,QWidget *parent = 0);
    ~DialogConnection();

public slots:
    QString GetAccountType();
    QString GetPassword();
    QString GetLogin();
    QString GetAPIKey();
    QString Getproxy();
    QString GetproxyHostName();
    quint16 GetproxyPort();
private slots:
    void on_pushButton_Connect_clicked();

//    void sslErrors(QNetworkReply *reply, QList<QSslError> ListError);
//    void replyencryptedFinished(QNetworkReply *reply);
//    void replyFinished(QNetworkReply *reply);

    void on_comboBox_AccountType_currentIndexChanged(const QString &arg1);
    void on_comboBox_proxy_activated(const QString &arg1);

private:
    Ui::DialogConnection *ui;
    QSettings *GUI_Settings;

    QString ig_username;
    QString ig_password;
    QString ig_API;
    QString ig_AccountType;

    QString mainUrl;
    QNetworkProxy proxy;

    QString rawHeader_X_SECURITY_TOKEN;
    QString rawHeader_CST;
    QUrl url;

signals:
    void write_LOG(int,QTime,QString);
    void RequestConnection();
    void ConnectionResult (bool IsConnected,QString currentrawHeader,QString rawHeader_CST,AccountInfo CurrentAccount);
};

#endif // DIALOGCONNECTION_H
