#include "dialogconnection.h"
#include "ui_dialogconnection.h"

#include <QCryptographicHash>



DialogConnection::DialogConnection(QSettings *Settings,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnection)
{
    ui->setupUi(this);
    GUI_Settings = Settings;



    //Reload saved parameters
    ig_AccountType = GUI_Settings->value("IG/AccountType","DEMO").toString();
    ui->comboBox_AccountType->setCurrentText(ig_AccountType);

    ui->lineEdit_login->setText(GUI_Settings->value("IG_" + ig_AccountType + "/identifier").toString());

    QString ig_password_encrypted = GUI_Settings->value("IG_" + ig_AccountType + "/password").toString();
    QByteArray ba;
    ba.append(ig_password_encrypted);
    ig_password =  QByteArray::fromBase64(ba);
    ui->lineEdit_password->setText(ig_password);


    QString ig_api_encrypted = GUI_Settings->value("IG_" + ig_AccountType + "/API").toString();
    QByteArray ba2;
    ba2.append(ig_api_encrypted);
    ig_API =  QByteArray::fromBase64(ba2);
    ui->lineEdit_api->setText(ig_API);


    QString ProxyType = GUI_Settings->value("Proxy","No Proxy").toString();
    int index = ui->comboBox_proxy->findText(ProxyType);
    if (index < 0)
        index = 0;
    ui->comboBox_proxy->setCurrentIndex(index);

    on_comboBox_proxy_activated(ui->comboBox_proxy->currentText());


}

DialogConnection::~DialogConnection()
{
    delete ui;
}

void DialogConnection::on_pushButton_Connect_clicked()
{
    ig_username = ui->lineEdit_login->text();
    ig_password = ui->lineEdit_password->text();
    ig_API = ui->lineEdit_api->text();

    GUI_Settings->setValue("IG/AccountType",ui->comboBox_AccountType->currentText());

    if (!ig_username.isEmpty())
        GUI_Settings->setValue("IG_"+ig_AccountType+"/identifier",ig_username);


    if (!ig_password.isEmpty())
    {
        QString ig_password_encrypted;// = QString(QCryptographicHash::hash((ig_password.toUtf8()),QCryptographicHash::Md5).toHex());
        QByteArray ba;
        ba.append(ig_password);
        ig_password_encrypted =  ba.toBase64();
        GUI_Settings->setValue("IG_"+ig_AccountType+"/password",ig_password_encrypted);
    }
    if (!ig_API.isEmpty())
    {
        QString ig_API_encrypted;// = QString(QCryptographicHash::hash((ig_API.toUtf8()),QCryptographicHash::Md5).toHex());
        QByteArray ba;
        ba.append(ig_API);
        ig_API_encrypted =  ba.toBase64();
        GUI_Settings->setValue("IG_"+ig_AccountType+"/API",ig_API_encrypted);
    }


     GUI_Settings->setValue("Proxy",ui->comboBox_proxy->currentText());

    emit RequestConnection();

}



QString DialogConnection::GetAccountType()
{
   return ui->comboBox_AccountType->currentText();
}

QString DialogConnection::GetLogin()
{
   return ui->lineEdit_login->text();
}

QString DialogConnection::GetproxyHostName()
{
   return ui->lineEdit_proxy->text();
}

quint16 DialogConnection::GetproxyPort()
{
   return ui->spinBox_ProxyPort->value();
}

QString DialogConnection::GetPassword()
{
   return ui->lineEdit_password->text();
}

QString DialogConnection::GetAPIKey()
{
   return ui->lineEdit_api->text();
}

QString DialogConnection::Getproxy()
{
   return ui->comboBox_proxy->currentText();
}





void DialogConnection::on_comboBox_AccountType_currentIndexChanged(const QString &arg1)
{
    ig_AccountType = arg1;
    GUI_Settings->setValue("IG/AccountType",ig_AccountType);

    QString ig_password_encrypted = GUI_Settings->value("IG_" + ig_AccountType + "/password").toString();
    QByteArray ba;
    ba.append(ig_password_encrypted);
    ig_password =  QByteArray::fromBase64(ba);
    ui->lineEdit_password->setText(ig_password);


    QString ig_api_encrypted = GUI_Settings->value("IG_" + ig_AccountType + "/API").toString();
    QByteArray ba2;
    ba2.append(ig_api_encrypted);
    ig_API =  QByteArray::fromBase64(ba2);
    ui->lineEdit_api->setText(ig_API);


    ui->lineEdit_login->setText(GUI_Settings->value("IG_" + ig_AccountType + "/identifier").toString());

}


void DialogConnection::on_comboBox_proxy_activated(const QString &arg1)
{
    if (arg1 == "User Proxy")
    {
        ui->lineEdit_proxy->setVisible(true);
        ui->spinBox_ProxyPort->setVisible(true);

    }
    else
    {
         ui->lineEdit_proxy->setVisible(false);
         ui->spinBox_ProxyPort->setVisible(false);
    }
}
