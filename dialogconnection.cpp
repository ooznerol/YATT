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



    //DEmo account
    ui->lineEdit_login_Demo->setText(GUI_Settings->value("IG_DEMO/identifier").toString());

    QString ig_password_encrypted = GUI_Settings->value("IG_DEMO/password").toString();
    QByteArray ba;
    ba.append(ig_password_encrypted);
    ig_password =  QByteArray::fromBase64(ba);
    ui->lineEdit_password_Demo->setText(ig_password);


    QString ig_api_encrypted = GUI_Settings->value("IG_DEMO/API").toString();
    QByteArray ba2;
    ba2.append(ig_api_encrypted);
    ig_API =  QByteArray::fromBase64(ba2);
    ui->lineEdit_api_Demo->setText(ig_API);

    //Real Account
    ui->lineEdit_login_Real->setText(GUI_Settings->value("IG_REAL/identifier").toString());

    ig_password_encrypted = GUI_Settings->value("IG_REAL/password").toString();
    ba.clear();
    ba.append(ig_password_encrypted);
    ig_password =  QByteArray::fromBase64(ba);
    ui->lineEdit_password_Real->setText(ig_password);

    ig_api_encrypted = GUI_Settings->value("IG_REAL/API").toString();
    ba2.clear();
    ba2.append(ig_api_encrypted);
    ig_API =  QByteArray::fromBase64(ba2);
    ui->lineEdit_api_Real->setText(ig_API);


    QString ProxyType = GUI_Settings->value("Connection/Proxy","No Proxy").toString();
    int index = ui->comboBox_proxy->findText(ProxyType);
    if (index < 0)
        index = 0;
    ui->comboBox_proxy->setCurrentIndex(index);

    ui->radioButtonAutoConnect->setChecked(GUI_Settings->value("Connection/AutoConnect").toBool());

    on_comboBox_proxy_activated(ui->comboBox_proxy->currentText());


}

DialogConnection::~DialogConnection()
{
    delete ui;
}

void DialogConnection::on_pushButton_Connect_clicked()
{
    GUI_Settings->setValue("IG/AccountType",ui->comboBox_AccountType->currentText());

    if (ui->comboBox_AccountType->currentText() == "REAL")
    {
        FillAccountInfo("REAL");

        if (!ui->lineEdit_login_Real->text().isEmpty())
            GUI_Settings->setValue("IG_REAL/identifier",ui->lineEdit_login_Real->text());


        if (!ui->lineEdit_password_Real->text().isEmpty())
        {
            QString ig_password_encrypted;
            QByteArray ba;
            ba.append(ui->lineEdit_password_Real->text());
            ig_password_encrypted =  ba.toBase64();
            GUI_Settings->setValue("IG_REAL/password",ig_password_encrypted);
        }
        if (!ui->lineEdit_api_Real->text().isEmpty())
        {
            QString ig_API_encrypted;
            QByteArray ba;
            ba.append(ui->lineEdit_api_Real->text());
            ig_API_encrypted =  ba.toBase64();
            GUI_Settings->setValue("IG_REAL/API",ig_API_encrypted);
        }
    }
    else
    {
        FillAccountInfo("DEMO");

        if (!ui->lineEdit_login_Demo->text().isEmpty())
            GUI_Settings->setValue("IG_DEMO/identifier",ui->lineEdit_login_Demo->text());


        if (!ui->lineEdit_password_Demo->text().isEmpty())
        {
            QString ig_password_encrypted;
            QByteArray ba;
            ba.append(ui->lineEdit_password_Demo->text());
            ig_password_encrypted =  ba.toBase64();
            GUI_Settings->setValue("IG_DEMO/password",ig_password_encrypted);
        }
        if (!ui->lineEdit_api_Demo->text().isEmpty())
        {
            QString ig_API_encrypted;
            QByteArray ba;
            ba.append(ui->lineEdit_api_Demo->text());
            ig_API_encrypted =  ba.toBase64();
            GUI_Settings->setValue("IG_DEMO/API",ig_API_encrypted);
        }

    }

    GUI_Settings->setValue("Connection/Proxy",ui->comboBox_proxy->currentText());


    struct StructProxy CurrentProxy;
    CurrentProxy.Type = ui->comboBox_proxy->currentText();
    CurrentProxy.HostName = ui->lineEdit_proxy->text();
    CurrentProxy.Port = ui->spinBox_ProxyPort->value();





    emit RequestConnection(ui->comboBox_AccountType->currentText());

}

void DialogConnection::FillAccountInfo(QString Type)
{
   CurrentAccount.Type = Type;
   if (Type == "REAL")
   {

       CurrentAccount.Id = ui->lineEdit_login_Real->text();
       CurrentAccount.password = ui->lineEdit_password_Real->text();
       CurrentAccount.api = ui->lineEdit_api_Real->text();
   }
   else
   {
       CurrentAccount.Id = ui->lineEdit_login_Demo->text();
       CurrentAccount.password = ui->lineEdit_password_Demo->text();
       CurrentAccount.api = ui->lineEdit_api_Demo->text();
   }

}

struct StructAccountInfo DialogConnection::GetAccountDemoInfo()
{
   FillAccountInfo("DEMO");
   return CurrentAccount;
}

struct StructAccountInfo DialogConnection::GetAccountRealInfo()
{
    FillAccountInfo("REAL");
    return CurrentAccount;
}


QString DialogConnection::GetAccountType()
{
   return ui->comboBox_AccountType->currentText();
}


struct StructProxy DialogConnection::Getproxy()
{
   struct StructProxy CurrentProxy;
   CurrentProxy.Type = ui->comboBox_proxy->currentText();
   CurrentProxy.HostName = ui->lineEdit_proxy->text();
   CurrentProxy.Port = ui->spinBox_ProxyPort->value();

   return CurrentProxy;
}

void DialogConnection::SetAccountType(QString Type)
{
   int index = ui->comboBox_AccountType->findText(Type);
   if (index < 0 )
       index = 0;
   ui->comboBox_AccountType->setCurrentIndex(index);

   ig_AccountType = Type;
   GUI_Settings->setValue("IG/AccountType",ig_AccountType);
}





void DialogConnection::on_comboBox_AccountType_currentIndexChanged(const QString &arg1)
{
    ig_AccountType = arg1;
    GUI_Settings->setValue("IG/AccountType",ig_AccountType);

//    QString ig_password_encrypted = GUI_Settings->value("IG_" + ig_AccountType + "/password").toString();
//    QByteArray ba;
//    ba.append(ig_password_encrypted);
//    ig_password =  QByteArray::fromBase64(ba);
//    ui->lineEdit_password->setText(ig_password);


//    QString ig_api_encrypted = GUI_Settings->value("IG_" + ig_AccountType + "/API").toString();
//    QByteArray ba2;
//    ba2.append(ig_api_encrypted);
//    ig_API =  QByteArray::fromBase64(ba2);
//    ui->lineEdit_api->setText(ig_API);


//    ui->lineEdit_login->setText(GUI_Settings->value("IG_" + ig_AccountType + "/identifier").toString());

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

void DialogConnection::on_radioButtonAutoConnect_clicked(bool checked)
{
    GUI_Settings->setValue("Connection/AutoConnect",checked);
}
