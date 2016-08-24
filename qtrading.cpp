#include "qtrading.h"
#include "ui_qtrading.h"
#include "QDebug"
#include "sleeper.h"




int factor = 10;


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


    //QDate StartDate = GUI_Settings->value("StartDate",QDate::currentDate()).toDate();

    write_LOG(LOG_NORMAL,QTime::currentTime(),"ini path : " + GUI_Settings->fileName() );
    ui->dateEdit_StartDate->setDate(QDate::currentDate());
    ui->dateEdit_EndDate->setDate(QDate::currentDate());


    //connect signal slot
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(manager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    connect(manager,SIGNAL(encrypted(QNetworkReply*)),this,SLOT(replyencryptedFinished(QNetworkReply*)));





    //Model trades
    modeltrades = new modelTrades(this);
    ui->tableView->setModel(modeltrades);
    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(15);

    //Chat
    chart = new QChart();
    chart->legend()->setVisible(false);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout_chart->addWidget(chartView);

     //Dialog connection
     dialogConnection = new DialogConnection(GUI_Settings,this);
     connect(dialogConnection,SIGNAL(write_LOG(int,QTime,QString)),this,SLOT(write_LOG(int,QTime,QString)));
     connect(dialogConnection,SIGNAL(RequestConnection()),this,SLOT(Connection_Request()));


     //Disable button
     ui->pushButton_Get->setEnabled(false);
     ui->dateEdit_StartDate->setEnabled(false);
     ui->dateEdit_EndDate->setEnabled(false);

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

void Qtrading::Connection_LightStreamerRequest()
{
    url.setUrl(ig_lightstreamerEndPoint + "/lightstreamer/create_session.txt"); //
    write_LOG(LOG_NORMAL,QTime::currentTime(),"try to connect to lightstreamer: " + url.url() );
    ui->textEdit->append("\nlightstreamer connection");

    QJsonObject Data;
    Data["LS_user"] = AccountID;
    qDebug() << "LS_user="  << AccountID;

    //lightStreamer PAssword = CST-CST token|XST-X-SECURITY-TOKEN
//    QString CST = "CST-" + rawHeader_CST;
//    QString X_SECURITY_TOKEN = "XST-" +  rawHeader_X_SECURITY_TOKEN;
    //QString ls_password = CST +"|"+ X_SECURITY_TOKEN;

    QString ls_password ="CST-" + rawHeader_CST + "|XST-" + rawHeader_X_SECURITY_TOKEN;
    qDebug() << "LS_password="  << ls_password;
    Data["LS_password"] = ls_password/*ig_password*/;

//    Data["LS_adapter_set"] = "";
    QByteArray jsonPost = QJsonDocument(Data).toJson();
    CurrentRequest = RequestConnectionLightStreamer;

    QNetworkRequest requestConnect;
    requestConnect.setUrl(url);
    requestConnect.setRawHeader("Content-Type", "application/json; charset=UTF-8");
    requestConnect.setRawHeader("Accept", "application/json; charset=UTF-8");
//    requestConnect.setRawHeader("X-IG-API-KEY", ig_API.toUtf8());
//    requestConnect.setRawHeader("Version", "2");
    manager->post(requestConnect, jsonPost);
}



#if 1
void Qtrading::Connection_Request()
{
    ui->textEdit->clear();
    ui->textEdit->append("\nig connection\n");

    ig_username = dialogConnection->GetLogin();
    ig_password = dialogConnection->GetPassword();
    ig_API = dialogConnection->GetAPIKey();

    //Set Proxy settings
    QString Proxy = dialogConnection->Getproxy();

    if (Proxy == "Sytem Proxy")
        QNetworkProxyFactory::setUseSystemConfiguration(true);
    else if (Proxy == "User Proxy")
    {
        QString ProxyHostName = dialogConnection->GetproxyHostName();
        quint16 ProxyPort = dialogConnection->GetproxyPort();
        QNetworkProxy proxy;
        proxy.setHostName(ProxyHostName);
        proxy.setPort(ProxyPort);

        QNetworkProxy::setApplicationProxy(proxy);
        manager->setProxy(proxy);
    }
    else
    {
        QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
        manager->setProxy(QNetworkProxy::NoProxy);
    }




    ig_AccountType = dialogConnection->GetAccountType();

    if (ig_AccountType == "REAL")
        url.setUrl("https://api.ig.com/gateway/deal/session");
    else
        url.setUrl("https://demo-api.ig.com/gateway/deal/session");
    write_LOG(LOG_NORMAL,QTime::currentTime(),"try to connect to : " + url.url() );


#if 0
    //instanciates new LSClient object with connection parameters
    lsclient* lsc = new lsclient(url.url(), ig_username, ig_password);

    //Adds a new subscription with 2 items to the client.
     lsc->addSubscription(new LSSubscription("MARKET",
                                             {"MARKET:IX.D.CAC.IMF.IP",
                                              "MARKET:IX.D.DAX.IMF.IP"},
                                             {"BID","OFFER"}));
#endif

#if 1
    QNetworkRequest requestConnect;
    requestConnect.setUrl(url);
    requestConnect.setRawHeader("Content-Type", "application/json; charset=UTF-8");
    requestConnect.setRawHeader("Accept", "application/json; charset=UTF-8");
    requestConnect.setRawHeader("X-IG-API-KEY", ig_API.toUtf8());
    requestConnect.setRawHeader("Version", "2");

    QJsonObject Data;
    Data["identifier"] = ig_username;
    Data["password"] = ig_password;
    QByteArray jsonPost = QJsonDocument(Data).toJson();
    CurrentRequest = RequestConnection;
    manager->post(requestConnect, jsonPost);


#endif
}

#endif
void Qtrading::replyencryptedFinished(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();
    qDebug() << "replyencryptedFinished : " << Answer;
}

void Qtrading::ProcessConnectionLightStreamerRequestAnswer(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();
    ui->textEdit->append("\nConnectionLightStreamerRequestAnswer\n");
    if (Answer.contains("ERROR"))
    {
         write_LOG(LOG_ERROR,QTime::currentTime(),"Connection to LightStreamer KO");
    }
    else
    {
         write_LOG(LOG_SUCCESS,QTime::currentTime(),"Connection to LightStreamer OK");
    }
    ui->textEdit->append(Answer);
    CurrentRequest = NoRequest;
}




void Qtrading::ProcessConnectionRequestAnswer(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();

    QJsonParseError *JSonError = nullptr;
    QJsonDocument JsonDOc = QJsonDocument::fromJson(Answer,JSonError);
    QJsonObject JsonObj = JsonDOc.object();

    bool ConnectionOK = JsonObj.contains("accountInfo");


    QJsonValue errorCode = JsonObj.value("errorCode");

    if (ConnectionOK)
    {
        //GEt Account ionformation and fill gui
        QJsonObject accountInfo = JsonObj["accountInfo"].toObject();
        QJsonValue CashAvailable = accountInfo.value("available");
        ui->lineEdit_AccountCashAvailable->setText(QString::number(CashAvailable.toDouble(),'f',2));

        QJsonValue balance = accountInfo.value("balance");
        ui->lineEdit_AccountBalance->setText(QString::number(balance.toDouble(),'f',2));

        QJsonValue profitLoss = accountInfo.value("profitLoss");
        ui->lineEdit_AccountProfitLoss->setText(QString::number(profitLoss.toDouble(),'f',2));

        QJsonValue currentAccountId = JsonObj.value("currentAccountId");
        AccountID = currentAccountId.toString();
        ui->lineEdit_AccountID->setText((AccountID));

        //Get security header
        QList<QByteArray> rawHeaderList = reply->rawHeaderList();
        foreach(QByteArray currentrawHeader , rawHeaderList)
        {
            //qDebug() << "Reply->rawHeaderList=" << currentrawHeader;
            if (currentrawHeader == "X-SECURITY-TOKEN")
                rawHeader_X_SECURITY_TOKEN = reply->rawHeader("X-SECURITY-TOKEN");
            if (currentrawHeader == "CST")
                rawHeader_CST = reply->rawHeader("CST");
        }


        QJsonValue currentAccountlightstreamerEndPoint = JsonObj.value("lightstreamerEndpoint");
        ig_lightstreamerEndPoint = currentAccountlightstreamerEndPoint.toString();

        ui->pushButton_Get->setEnabled(true);
        ui->dateEdit_StartDate->setEnabled(true);
        ui->dateEdit_EndDate->setEnabled(true);

        dialogConnection->hide();
        write_LOG(LOG_SUCCESS,QTime::currentTime(),"Connection to IG OK");

        CurrentRequest = RequestConnectionLightStreamer;
        Connection_LightStreamerRequest();

    }
    else
    {
        QString error = errorCode.toString() ;
        write_LOG(LOG_ERROR,QTime::currentTime(),"Connection to IG failed :" + error);
        CurrentRequest = NoRequest;
    }

    ui->textEdit->append("\nProcessConnectionRequestAnswer\n");
    ui->textEdit->append(Answer);
}


void Qtrading::replyFinished(QNetworkReply *reply)
{
    if (CurrentRequest == RequestConnection)
        ProcessConnectionRequestAnswer(reply);
    else if (CurrentRequest == RequestGetTransactionList)
        ProcessGetTransactionListRequestAnswer(reply);
    else if (CurrentRequest == RequestConnectionLightStreamer)
        ProcessConnectionLightStreamerRequestAnswer(reply);

    else
    {
        QByteArray Answer = reply->readAll();
        ui->textEdit->append("\nunknowRequestAnswer");
        ui->textEdit->append(Answer);
    }
}


void Qtrading::ProcessGetTransactionListRequestAnswer(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();

    QJsonParseError *JSonError = nullptr;
    QJsonDocument JsonDOc = QJsonDocument::fromJson(Answer,JSonError);
    QJsonObject JsonObj = JsonDOc.object();
    QJsonArray jsonArray = JsonObj["transactions"].toArray();

    QJsonObject meta = JsonObj["metadata"].toObject();
    QJsonObject pageData = meta["pageData"].toObject();

    QJsonValue pageNumber = pageData.value("pageNumber");
    CurrentPageNumber = pageNumber.toInt();

    QJsonValue totalPages = pageData.value("totalPages");
    TotalPages = totalPages.toInt();




    QLineSeries *seriesTrades = new QLineSeries();
    QLineSeries *seriesZero = new QLineSeries();

    QScatterSeries* Scatterseries = new QScatterSeries();
    Scatterseries->setMarkerSize(5.0);

    float sum = 0;
    bool ok;




    foreach (const QJsonValue & value, jsonArray) {

        TransactionStruct CurrentTransaction;

        QJsonObject obj = value.toObject();
        CurrentTransaction.date = obj["date"].toString();
        CurrentTransaction.dateUTC = obj["dateUtc"].toString();
        CurrentTransaction.instrumentName = obj["instrumentName"].toString();
        CurrentTransaction.period = obj["period"].toString();
        CurrentTransaction.profitAndLoss = obj["profitAndLoss"].toString();
        CurrentTransaction.transactionType = obj["transactionType"].toString();
        CurrentTransaction.reference = obj["reference"].toString();
        CurrentTransaction.openLevel = obj["openLevel"].toString().toDouble(&ok);
        CurrentTransaction.closeLevel = obj["closeLevel"].toString().toDouble(&ok);

        double delta = CurrentTransaction.openLevel - CurrentTransaction.closeLevel;
        delta = fabs (delta);
        if (CurrentTransaction.profitAndLoss.contains("-"))
            delta = -delta;

        if (delta < 0)
        {
            numberOftradesLost++;
            numberOfPointsLost += delta;
        }
        else  if (delta > 0)
        {
            numberOftradesWon++;
            numberOfPointsWon += delta;
        }
        else
        {
            numberOftradesFlat++;
        }

        CurrentTransaction.deltaPoint = delta;
        CurrentTransaction.currency = obj["currency"].toString();
        CurrentTransaction.size = obj["size"].toString();
        CurrentTransaction.cashTransaction = obj["cashTransaction"].toBool();

        ListOfTransaction.append(CurrentTransaction);
        modeltrades->addOneEntry(CurrentTransaction);
//        ui->tableView->setRowHeight(2,20);
    }

    if (CurrentPageNumber != TotalPages && TotalPages!=0)
    {
        //Get others pages
        GetListOfTrades(CurrentPageNumber + 1);
    }
    else
    {
        ui->spinBox_TradesWon->setValue(numberOftradesWon);
        ui->spinBox_TradesLost->setValue(numberOftradesLost);
        ui->spinBox_TradesFlat->setValue(numberOftradesFlat);


        ui->doubleSpinBox_PointsLost->setValue(numberOfPointsLost);
        ui->doubleSpinBox_PointsWon->setValue(numberOfPointsWon);
        ui->doubleSpinBox_PointsTotal->setValue(numberOfPointsWon + numberOfPointsLost);

        seriesTrades->append(0,0);
        seriesZero->append(0,0);

        int index = 1 ;
        for (int i = ListOfTransaction.size(); i > 0 ; i--)
        {
            sum += ListOfTransaction.at(i-1).deltaPoint;
            seriesTrades->append(index,sum);
            seriesZero->append(index,0);
            Scatterseries->append(index,sum);
            index++;
        }

        chart->addSeries(seriesTrades);
        chart->addSeries(seriesZero);
        chart->addSeries(Scatterseries);

        chart->createDefaultAxes();



        chart->axisX()->setTitleText("trades");
        chart->axisY()->setTitleText("points");

        ui->tableView->resizeColumnsToContents();

        write_LOG(LOG_SUCCESS,QTime::currentTime(),"Get " + QString::number(ListOfTransaction.size()) + " transactions");



        CurrentRequest = NoRequest;
    }

    ui->textEdit->append(Answer);
}

void Qtrading::GetListOfTrades(int page)
{
    QNetworkRequest request;

    request.setRawHeader("Content-Type", "application/json; charset=UTF-8");
    request.setRawHeader("Accept", "application/json; charset=UTF-8");
    request.setRawHeader("X-IG-API-KEY", ig_API.toUtf8());
    request.setRawHeader("Version", "2");

    QString StartDate = ui->dateEdit_StartDate->date().toString("yyyy-MM-dd");
    QString EndDate = ui->dateEdit_EndDate->date().toString("yyyy-MM-dd");

    if (ig_AccountType == "REAL")
        url.setUrl("https://api.ig.com/gateway/deal/history/transactions?type=ALL&from=" + StartDate + "&to=" + EndDate + "&pageNumber=" + QString::number(page));
    else
        url.setUrl("https://demo-api.ig.com/gateway/deal/history/transactions?type=ALL&from=" + StartDate + "&to=" + EndDate + "&pageNumber=" + QString::number(page));

    request.setUrl(url);

    request.setRawHeader("X-SECURITY-TOKEN", rawHeader_X_SECURITY_TOKEN.toUtf8());
    request.setRawHeader("CST", rawHeader_CST.toUtf8());

    CurrentRequest = RequestGetTransactionList;

    manager->get(request);
}

void Qtrading::on_pushButton_Get_clicked()
{
      modeltrades->Clear();
      chart->removeAllSeries();

      ListOfTransaction.clear();

      numberOftradesWon = 0;
      numberOftradesLost = 0;
      numberOftradesFlat = 0;

      numberOfPointsLost = 0 ;
      numberOfPointsTotal = 0 ;
      numberOfPointsWon = 0;
      GetListOfTrades(1);
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

void Qtrading::on_actionConnect_triggered()
{
    dialogConnection->show();
}



