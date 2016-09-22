//    This file is part of YATT.

//    YATT is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    YATT is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with YATT.  If not, see <http://www.gnu.org/licenses/>. 2

#include "igls.h"

IGLS::IGLS(QObject *parent) : QObject(parent)
{
    //connect signal slot
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(manager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    connect(manager,SIGNAL(encrypted(QNetworkReply*)),this,SLOT(replyencryptedFinished(QNetworkReply*)));

    EventLoopWaitIGConnectAnswer = new QEventLoop(this);
    EventLoopWaitIGGetTrade= new QEventLoop(this);

    CurrentStartDate.setDate(QDate(0,0,0));
    CurrentEndDate.setDate(QDate(0,0,0));
}

void IGLS::sslErrors(QNetworkReply* reply,QList<QSslError> ListError)
{
    reply->ignoreSslErrors();
    foreach (QSslError error, ListError)
    {
        qDebug() << "SSL ERRORS="  << error.errorString();
    };
}


void IGLS::Connection(struct StructProxy CurrentProxy,struct StructAccountInfo CurrentAccount)
{
#if 1
    //ui->textEditDebug->clear();
    emit Write_DEBUG("\nig connection to " + CurrentProxy.Type + "\n");


    bLightStreamerSubscribeStatus = false;




    if (CurrentProxy.Type == "Sytem Proxy")
        QNetworkProxyFactory::setUseSystemConfiguration(true);
    else if (CurrentProxy.Type == "User Proxy")
    {

        QNetworkProxy proxy;
        proxy.setHostName(CurrentProxy.HostName);
        proxy.setPort(CurrentProxy.Port);

        QNetworkProxy::setApplicationProxy(proxy);
        manager->setProxy(proxy);
    }
    else
    {
        QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
        manager->setProxy(QNetworkProxy::NoProxy);
    }

    if (CurrentAccount.Type == "REAL")
    {
        URL_IG = "https://api.ig.com";
    }
    else
    {
        URL_IG = "https://demo-api.ig.com";
    }

    ig_username = CurrentAccount.Id;
    ig_password = CurrentAccount.password;
    ig_API = CurrentAccount.api;


    url.setUrl(URL_IG + "/gateway/deal/session");

    emit write_LOG(LOG_NORMAL,QTime::currentTime(),"try to connect to : " + url.url() );


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


    IsConnectedToIG = false;
    IsConnectedToLS= false;
    IsSubscribeToLS= false;
    IsBindToLS= false;




    //Try to connect to IG
    CurrentRequest = RequestConnectionIG;
    manager->post(requestConnect, jsonPost);
    //wair for Answer
    emit write_LOG(LOG_NORMAL,QTime::currentTime(),"WaitIGConnectAnswer...." );
    EventLoopWaitIGConnectAnswer->exec();

    if (IsConnectedToIG == true)
    {
        //GEt first page of trade

       // QDateTime CurrentDate =QDateTime::currentDateTime().toString("yyyy-MM-dd");

        if (CurrentStartDate.date().isNull())
            CurrentStartDate = QDateTime::currentDateTime();
        if (CurrentEndDate.date().isNull())
            CurrentEndDate = QDateTime::currentDateTime();

        GetListOfTradesRequest(CurrentStartDate,CurrentEndDate);
        emit write_LOG(LOG_NORMAL,QTime::currentTime(),"WaitIGGetTradeAnswer...." );
        EventLoopWaitIGGetTrade->exec();
    }

#endif

}


void IGLS::onRequestCompleted()
{
    qDebug() << "onRequestCompleted" ;
}





void IGLS::replyencryptedFinished(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();
    qDebug() << "replyencryptedFinished : " << Answer;
}


void IGLS::ProcessConnectionRequestAnswer(QNetworkReply *reply)
{
    QByteArray Answer = reply->readAll();

    QJsonParseError *JSonError = nullptr;
    QJsonDocument JsonDOc = QJsonDocument::fromJson(Answer,JSonError);
    QJsonObject JsonObj = JsonDOc.object();

    IsConnectedToIG = JsonObj.contains("accountInfo");


    QJsonValue errorCode = JsonObj.value("errorCode");

    if (IsConnectedToIG)
    {
        //GEt Account information and fill gui
        QJsonObject accountInfo = JsonObj["accountInfo"].toObject();
        QJsonValue CashAvailable = accountInfo.value("available");


        QJsonValue balance = accountInfo.value("balance");

        QJsonValue profitLoss = accountInfo.value("profitLoss");

        QJsonValue currentAccountId = JsonObj.value("currentAccountId");
        AccountID = currentAccountId.toString();

        emit AccountInfoUpdate(QString::number(CashAvailable.toDouble(),'f',2),
                               QString::number(balance.toDouble(),'f',2),
                               QString::number(profitLoss.toDouble(),'f',2),
                               AccountID
                               );



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
        URL_LightStreamerEndPoint = currentAccountlightstreamerEndPoint.toString();

        emit ConnectedToIG (true);
        //mainwindowhistory->EnableButton(true);

//        mainwindowtradinglive->EnableTrading(true);

        emit ConnectedToIG(true);

        emit write_LOG(LOG_SUCCESS,QTime::currentTime(),"Connection to IG OK");

        CurrentRequest = RequestConnectionLightStreamer;
    }
    else
    {
        QString error = errorCode.toString() ;
        emit write_LOG(LOG_ERROR,QTime::currentTime(),"Connection to IG failed :" + error);
        CurrentRequest = NoRequest;
    }

    emit Write_DEBUG("\nProcessConnectionRequestAnswer\n");
    emit Write_DEBUG(Answer);
}



void IGLS::replyFinished(QNetworkReply *reply)
{


    QString Type;
    switch (CurrentRequest)
    {
        case RequestConnectionIG:
            Type = " COnnection to IG";
        break;
        case RequestGetTransactionList:
            Type = " GEt ransaction List from IG";
            break;
        default:
            Type = " unknow ";
            break;

    }
    qDebug() << "replyFinished : " << "type=" << Type;

    QVariant statusCodeV =  reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QVariant statusCodeV2 =  reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute);
    qDebug() << "replyFinished : " << "statusCodeV=" << statusCodeV << " : " << statusCodeV2;




    if (CurrentRequest == RequestConnectionIG)
    {
        ProcessConnectionRequestAnswer(reply);
        EventLoopWaitIGConnectAnswer->exit();
    }
    else if (CurrentRequest == RequestGetTransactionList)
    {
        emit write_LOG(LOG_NORMAL,QTime::currentTime(),"GetListOfTrades Answer received -> stop EventLoopWaitIGGetTrade");
        ProcessGetTransactionListRequestAnswer(reply);
        EventLoopWaitIGGetTrade->exit();
    }
    else
    {
        QByteArray Answer = reply->readAll();
        emit Write_DEBUG("\nunknowRequestAnswer");
        emit Write_DEBUG("statusCodeV=" + statusCodeV.toString() + " : " + statusCodeV2.toString());
        emit Write_DEBUG(Answer);

        qDebug() << "replyFinished Answer : " << Answer;
    }
}


void IGLS::ProcessGetTransactionListRequestAnswer(QNetworkReply *reply)
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


    emit TransactionListAvailable(jsonArray);

    if (CurrentPageNumber != TotalPages && TotalPages!=0)
    {
        //Get others pages
        GetListOfTrades(CurrentPageNumber + 1,CurrentStartDate ,CurrentEndDate);
    }
    else
    {
        //We have finished to get
        emit updateTradesInfo();
        CurrentRequest = NoRequest;
    }

    emit Write_DEBUG(Answer);


}

void IGLS::GetListOfTrades(int page,QDateTime StartDate ,QDateTime EndDate)
{
    emit write_LOG(LOG_NORMAL,QTime::currentTime(),"GetListOfTrades Send to IG from " + StartDate.toString("yyyy-MM-dd") + " to " + EndDate.toString("yyyy-MM-dd"));

    CurrentRequest = RequestGetTransactionList;

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json; charset=UTF-8");
    request.setRawHeader("Accept", "application/json; charset=UTF-8");
    request.setRawHeader("X-IG-API-KEY", ig_API.toUtf8());
    request.setRawHeader("Version", "2");
    request.setRawHeader("X-SECURITY-TOKEN", rawHeader_X_SECURITY_TOKEN.toUtf8());
    request.setRawHeader("CST", rawHeader_CST.toUtf8());


    url.setUrl(URL_IG + "/gateway/deal/history/transactions?type=ALL&from=" + StartDate.toString("yyyy-MM-dd") + "&to=" + EndDate.toString("yyyy-MM-dd") + "&pageNumber=" + QString::number(page));
    request.setUrl(url);
    manager->get(request);
}

void IGLS::GetListOfTradesRequest(QDateTime StartDate,QDateTime EndDate)
{
      CurrentStartDate = StartDate/*.toString("yyyy-MM-dd")*/;
      CurrentEndDate = EndDate/*.toString("yyyy-MM-dd")*/;

      GetListOfTrades(1,CurrentStartDate,CurrentEndDate);
}


