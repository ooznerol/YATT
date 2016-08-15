#include "qtrading.h"
#include "ui_qtrading.h"
#include "QDebug"
#include "sleeper.h"





modelTrades* modeltrades;

Qtrading::Qtrading(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qtrading)
{
    ui->setupUi(this);

   //create Log file
    QString application_dir = qApp->applicationDirPath();
    QString log_file_path = application_dir + "/Qtrading.log";
    createLogFile(log_file_path);


    //create ini file
    QString ini_name = "Qtrading";
    GUI_Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,"YATT",ini_name);


    //Reload saved parameters
    ig_AccountType = GUI_Settings->value("IG/AccountType","DEMO").toString();
    ui->comboBox_AccountType->setCurrentText(ig_AccountType);

    ui->lineEdit_login->setText(GUI_Settings->value("IG_" + ig_AccountType + "/identifier").toString());
    ui->lineEdit_password->setText(GUI_Settings->value("IG_" + ig_AccountType + "/password").toString());
    ui->lineEdit_api->setText(GUI_Settings->value("IG_" + ig_AccountType + "/API").toString());

    //QDate StartDate = GUI_Settings->value("StartDate",QDate::currentDate()).toDate();

    write_LOG(LOG_NORMAL,QTime::currentTime(),"ini path : " + GUI_Settings->fileName() );
    ui->dateEdit_StartDate->setDate(QDate::currentDate());
    ui->dateEdit_ENdDate->setDate(QDate::currentDate());


    //connect signal slot
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(manager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    connect(manager,SIGNAL(encrypted(QNetworkReply*)),this,SLOT(replyencryptedFinished(QNetworkReply*)));


    //Set Proxy settings
    QNetworkProxyFactory::setUseSystemConfiguration(true);




    QStringList HeaderLabels;

    HeaderLabels <<  "date" << "dateUTC" << "instrumentName" << "period" << "profitAndLoss" << "transactionType" << "reference";
    HeaderLabels << "openLevel" << "closeLevel" << "size" << "currency" << "cashTransaction";





    ui->tableWidget_trades->setHorizontalHeaderLabels(HeaderLabels);

    modeltrades = new modelTrades(0);

    modelTrades mymodel(0);
    ui->tableView->setModel(&mymodel);


    chart = new QChart();
    chart->legend()->hide();

    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

     ui->verticalLayout_chart->addWidget(chartView);



}

Qtrading::~Qtrading()
{
    delete ui;
}


void Qtrading::sslErrors(QNetworkReply* reply,QList<QSslError> ListError)
{
    reply->ignoreSslErrors();
    foreach (QSslError error, ListError) {

        qDebug() << "SSL ERRORS="  << error.errorString();

    };
}



void Qtrading::on_pushButton_Connect_clicked()
{
    ui->textEdit->clear();

    ig_username = ui->lineEdit_login->text();
    ig_password = ui->lineEdit_password->text();
    ig_API = ui->lineEdit_api->text();




    if (ui->comboBox_AccountType->currentText() == "REAL")
        url.setUrl("https://api.ig.com/gateway/deal/session");

    else
        url.setUrl("https://demo-api.ig.com/gateway/deal/session");


    write_LOG(LOG_NORMAL,QTime::currentTime(),"try to connect to : " + url.url() );

    if (!ig_username.isEmpty())
        GUI_Settings->setValue("IG_"+ig_AccountType+"/identifier",ig_username);

#if 0
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    hasher.addData(ig_password.toUtf8());
    QByteArray passprotected=hasher.result();
#endif
    if (!ig_password.isEmpty())
        GUI_Settings->setValue("IG_"+ig_AccountType+"/password",ig_password);
    if (!ig_API.isEmpty())
        GUI_Settings->setValue("IG_"+ig_AccountType+"/API",ig_API);



    QNetworkRequest requestConnect;
    requestConnect.setUrl(url);


    requestConnect.setRawHeader("Content-Type", "application/json; charset=UTF-8");
    requestConnect.setRawHeader("Accept", "application/json; charset=UTF-8");
    requestConnect.setRawHeader("X-IG-API-KEY", ig_API.toUtf8());
    requestConnect.setRawHeader("Version", "2");







    QJsonObject Data;
    Data["identifier"] = ig_username;
    Data["password"] = ig_password;
    QJsonDocument JsonDoc(Data);

    QByteArray jsonPost = QJsonDocument(Data).toJson();



   //QByteArray Data2 ( "{\"identifier\": \"" + ig_username + "\",\"password\": \"" + ig_password +"\"}\"");
   manager->post(requestConnect, jsonPost);

//   manager->get(QNetworkRequest(url));

}


void Qtrading::replyencryptedFinished(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();
    qDebug() << "replyencryptedFinished : " << Answer;
}


void Qtrading::replyFinished(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();





    //qDebug() << "Answer=" << Answer;

    QList<QByteArray> rawHeaderList = reply->rawHeaderList();

    foreach(QByteArray currentrawHeader , rawHeaderList)
    {
        //qDebug() << "Reply->rawHeaderList=" << currentrawHeader;
        if (currentrawHeader == "X-SECURITY-TOKEN")
            rawHeader_X_SECURITY_TOKEN = reply->rawHeader("X-SECURITY-TOKEN");
        if (currentrawHeader == "CST")
            rawHeader_CST = reply->rawHeader("CST");
    }

    ui->textEdit->setText(Answer);

    QJsonParseError *JSonError = nullptr;
    QJsonDocument JsonDOc = QJsonDocument::fromJson(Answer,JSonError);
    QJsonObject JsonObj = JsonDOc.object();
    QJsonArray jsonArray = JsonObj["transactions"].toArray();

    QJsonObject meta = JsonObj["metadata"].toObject();
    QJsonObject pages = meta["pages"].toObject();


    ListOfTransaction.clear();

    QLineSeries *series = new QLineSeries();

    float sum = 0;


    foreach (const QJsonValue & value, jsonArray) {

        TransactionStruct CurrentTransaction;

        QJsonObject obj = value.toObject();
        CurrentTransaction.date = obj["date"].toString();
        CurrentTransaction.dateUTC = obj["dateUTC"].toString();
        CurrentTransaction.instrumentName = obj["instrumentName"].toString();
        CurrentTransaction.period = obj["period"].toString();
        CurrentTransaction.profitAndLoss = obj["profitAndLoss"].toString();
        CurrentTransaction.transactionType = obj["transactionType"].toString();
        CurrentTransaction.reference = obj["reference"].toString();

        QString test = obj["openLevel"].toString();
        bool ok;
        CurrentTransaction.openLevel = test.toFloat(&ok);
        test.replace(".",",");

        CurrentTransaction.openLevel = test.toFloat(&ok);

        CurrentTransaction.openLevel = obj["openLevel"].toString().toFloat(&ok);

//        double test2 = obj["closeLevel"].toDouble();
//        qDebug()<< test2;
        CurrentTransaction.closeLevel = obj["closeLevel"].toString().toFloat(&ok);
        CurrentTransaction.currency = obj["currency"].toString();
        CurrentTransaction.size = obj["size"].toDouble();
        CurrentTransaction.cashTransaction = obj["cashTransaction"].toBool();

        ListOfTransaction.append(CurrentTransaction);

        ui->tableWidget_trades->insertRow(0);
        modeltrades->addOneEntry(CurrentTransaction);

        float DeltaPoint = CurrentTransaction.closeLevel-CurrentTransaction.openLevel;
        sum += DeltaPoint;

        series->append(ListOfTransaction.size(),sum);
 }

    chart->addSeries(series);
}




void Qtrading::on_pushButton_Get_clicked()
{

          QNetworkRequest request;



          request.setRawHeader("Content-Type", "application/json; charset=UTF-8");
          request.setRawHeader("Accept", "application/json; charset=UTF-8");
          request.setRawHeader("X-IG-API-KEY", ig_API.toUtf8());
          request.setRawHeader("Version", "2");

          QString StartDate = ui->dateEdit_StartDate->date().toString("yyyy-MM-dd");
          QString EndDate = ui->dateEdit_ENdDate->date().toString("yyyy-MM-dd");

          if (ig_AccountType == "REAL")
              url.setUrl("https://api.ig.com/gateway/deal/history/transactions?type=ALL&from=" + StartDate + "&to=" + EndDate);

          else
              url.setUrl("https://demo-api.ig.com/gateway/deal/history/transactions?type=ALL&from=" + StartDate + "&to=" + EndDate);





              request.setUrl(url);

          request.setRawHeader("X-SECURITY-TOKEN", rawHeader_X_SECURITY_TOKEN.toUtf8());
          request.setRawHeader("CST", rawHeader_CST.toUtf8());

          manager->get(request);
}

void Qtrading::on_comboBox_AccountType_currentIndexChanged(const QString &arg1)
{
    ig_AccountType = arg1;
    GUI_Settings->setValue("IG/AccountType",ig_AccountType);

    ui->lineEdit_login->setText(GUI_Settings->value("IG_" + ig_AccountType + "/identifier").toString());
    ui->lineEdit_password->setText(GUI_Settings->value("IG_" + ig_AccountType + "/password").toString());
    ui->lineEdit_api->setText(GUI_Settings->value("IG_" + ig_AccountType + "/API").toString());
}


void Qtrading::write_LOG(int debug_level, QTime timestamp, QString log)
{
#if 1
    QString type_field;
    QString message;

    QPalette p = ui->plainTextEdit->palette();

    QTextCharFormat tf;
    tf = ui->plainTextEdit->currentCharFormat();


    QColor textColor;


//    if (DEBUG_LEVEL <= debug_level)
//    {
         if (debug_level & LOG_ERROR)
         {
             type_field = "ERROR\t";
             textColor = QColor(Qt::red);
         }
         else if (debug_level & LOG_SUCCESS)
         {
             type_field = "SUCCESS\t";
             textColor = QColor(Qt::green);
         }
         else if (debug_level & LOG_DEBUG)
         {
             type_field = "DEBUG\t";
             textColor = QColor(Qt::gray);
         }
         else if (debug_level & LOG_WARNING)
         {
             type_field = "WARNING\t";
             textColor = QColor(255, 175, 0);
         }
         else
         {
             type_field = "INFO\t";
             textColor = QColor(Qt::black);
         }
//    }

    tf.setForeground(QBrush(textColor));

    ui->plainTextEdit->setCurrentCharFormat(tf);

    message = timestamp.toString("hh:mm:ss.zzz") + " : " +  type_field + ":" + log/* +  "\r\n"*/;

    ui->plainTextEdit->appendPlainText(message);



    QTextStream out(&log_file);
    out << message << endl;



    ui->plainTextEdit->moveCursor(QTextCursor::End);
    ui->plainTextEdit->ensureCursorVisible();
#endif

}


void Qtrading::createLogFile(QString log_file_path)
{
    log_file.setFileName(log_file_path);
    if (log_file.exists())
        log_file.remove();

    qDebug() << "Open/create log file " <<  log_file_path;

    bool log_is_open = log_file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);

    write_LOG(LOG_NORMAL,QTime::currentTime(),"log path : " + log_file_path );

    if (log_is_open)
        write_LOG(LOG_SUCCESS,QTime::currentTime(),"open/create log file SUCCESS" );
    else
        write_LOG(LOG_ERROR,QTime::currentTime(),"open/create log file FAILED" );



#if 0
    //Get operating System
    QString OPerating_system;

    #ifdef Q_WS_X11
    OPerating_system = "Linux";
    #endif

    #ifdef Q_WS_MAC
    OPerating_system = "Mac";
    #endif

    #ifdef Q_WS_QWS
    OPerating_system = "Embedded Linux";
    #endif

    #ifdef Q_WS_WIN
    switch(QSysInfo::windowsVersion())
    {
      case QSysInfo::WV_2000: OPerating_system =  "Windows 2000";
      case QSysInfo::WV_XP: OPerating_system = "Windows XP";
      case QSysInfo::WV_VISTA: OPerating_system = "Windows Vista";
      case QSysInfo::WV_WINDOWS7:OPerating_system = "windows 7";
      default:
        break;
    }
    #ifdef Q_OS_WIN64
        OPerating_system += " 64bit";
    #else
        OPerating_system += " 32bit";
    #endif

    #endif
    write_LOG(LOG_NORMAL,QTime::currentTime(),"OS = " + OPerating_system);
#endif
}
