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
//    QString GetPasswordREAL();
//    QString GetLoginREAL();
//    QString GetAPIKeyREAL();

//    QString GetPasswordDEMO();
//    QString GetLoginDEMO();
//    QString GetAPIKeyDEMO();

    struct StructProxy Getproxy();
    void SetAccountType(QString Type);
    StructAccountInfo GetAccountDemoInfo();
    StructAccountInfo GetAccountRealInfo();

private slots:
    void on_pushButton_Connect_clicked();
    void on_comboBox_AccountType_currentIndexChanged(const QString &arg1);
    void on_comboBox_proxy_activated(const QString &arg1);
    void on_radioButtonAutoConnect_clicked(bool checked);

    void FillAccountInfo(QString Type);
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
    struct StructAccountInfo CurrentAccount;

signals:
    void write_LOG(int,QTime,QString);
    void RequestConnection(QString);
    void ConnectionResult (bool IsConnected,QString currentrawHeader,QString rawHeader_CST,AccountInfo CurrentAccount);
};

#endif // DIALOGCONNECTION_H
