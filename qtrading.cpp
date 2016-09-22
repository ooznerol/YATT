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


#include "qtrading.h"
#include "ui_qtrading.h"
#include "QDebug"




Qtrading::Qtrading(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qtrading)

{

    ui->setupUi(this);

    this->setWindowTitle("YATT v1.00");



    //igls
    igls = new IGLS();

    //create ini file
    QString ini_name = "YATT";
    GUI_Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,"YATT",ini_name);


    //Load Size and Position
    bool ok;
    int width = GUI_Settings->value("MainWindow/width",this->size().width()).toUInt(&ok);
    int height = GUI_Settings->value("MainWindow/height",this->size().height()).toUInt(&ok);
    int x = GUI_Settings->value("MainWindow/x",this->x()).toUInt(&ok);
    int y = GUI_Settings->value("MainWindow/y",this->y()).toUInt(&ok);

    this->move(QPoint(x,y));
    this->resize(QSize(width,height));



    //Window log
    mainwindowlog = new MainWindowLog(GUI_Settings);
    connect(mainwindowlog,SIGNAL(MainWindowClosed(bool)),ui->actionLog,SLOT(setChecked(bool)));


    //Window log
    mainwindowlog->CreateLog();
    mainwindowlog->write_LOG(LOG_NORMAL,QTime::currentTime(),"ini path : " + GUI_Settings->fileName() );

    //Dialog connection
    dialogConnection = new DialogConnection(GUI_Settings,this);
    dialogConnection->setModal(true);
    connect(dialogConnection,SIGNAL(write_LOG(int,QTime,QString)),mainwindowlog,SLOT(write_LOG(int,QTime,QString)));
    connect(dialogConnection,SIGNAL(RequestConnection(QString)),this,SLOT(connectionRequest(QString)));


    //Window history
    mainwindowhistory = new MainWindowHistory(GUI_Settings);

    connect(mainwindowhistory,SIGNAL(MainWindowClosed(bool)),ui->actionHistory,SLOT(setChecked(bool)));
    ui->actionHistory->setChecked(GUI_Settings->value("GUI/MainWindowHistoryVisible",false).toBool());
    on_actionHistory_toggled(GUI_Settings->value("GUI/MainWindowHistoryVisible",false).toBool());
    connect(mainwindowhistory,SIGNAL(write_LOG(int,QTime,QString)),mainwindowlog,SLOT(write_LOG(int,QTime,QString)));


    //Timer to check Open position
    TimerGetOpenPosition = new QTimer (this);
    connect(TimerGetOpenPosition,SIGNAL(timeout()),igls,SLOT(GetOpenPosition()));
    TimerGetOpenPosition->setInterval(5000);
    //TimerGetOpenPosition->start();

    //connection beteween igls and other class
    connect(igls,SIGNAL(ConnectedToIG(bool)),this,SLOT(HideDialogConnection(bool)));
    connect(igls,SIGNAL(AccountInfoUpdate(QString,QString,QString,QString)),this,SLOT(AccountInfoUpdate(QString,QString,QString,QString)));


    connect(igls,SIGNAL(Write_DEBUG(QString)),mainwindowlog,SLOT(Write_DEBUG(QString)));
    connect(igls,SIGNAL(write_LOG(int,QTime,QString)),mainwindowlog,SLOT(write_LOG(int,QTime,QString)));

    connect(igls,SIGNAL(ConnectedToIG(bool)),mainwindowhistory,SLOT(EnableButton(bool)));
    connect(igls,SIGNAL(TransactionListAvailable(QJsonArray)),mainwindowhistory,SLOT(ParseTransactionList(QJsonArray)));
    connect(igls,SIGNAL(updateTradesInfo()),mainwindowhistory,SLOT(updateTradesInfo()));
    connect(igls,SIGNAL(AccountInfoUpdate(QString,QString,QString,QString)),mainwindowhistory,SLOT(updateAccoutnInfo(QString,QString,QString,QString)));
    connect(mainwindowhistory,SIGNAL(GetListOfTrades(QDateTime,QDateTime)),igls,SLOT(GetListOfTradesRequest(QDateTime,QDateTime)));


    //if not first run load parameter else see dialog connection
    if(GUI_Settings->contains("IG/AccountType"))
    {
         ig_AccountType = GUI_Settings->value("IG/AccountType","DEMO").toString();
         bool autoconnect = GUI_Settings->value("Connection/AutoConnect",false).toBool();
         if (autoconnect)
             connectionRequest(ig_AccountType);
    }
    else
    {
        this->setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                this->size(),
                qApp->desktop()->availableGeometry()
            ));
        dialogConnection->show();
    }



    //Window log
    on_actionLog_toggled(GUI_Settings->value("GUI/MainWindowLogVisible",false).toBool());
    ui->actionLog->setChecked(GUI_Settings->value("GUI/MainWindowLogVisible",false).toBool());


}

Qtrading::~Qtrading()
{

    qDebug() << "Qtrading delete enter";
    delete ui;
    qDebug() << "Qtrading delete exit";

}
void Qtrading::HideDialogConnection(bool status)
{
    if (status)
        dialogConnection->hide();
}

void Qtrading::SaveSizeAndPosition()
{
    GUI_Settings->setValue("MainWindow/width",this->size().width());
    GUI_Settings->setValue("MainWindow/height",this->size().height());
    GUI_Settings->setValue("MainWindow/x",this->x());
    GUI_Settings->setValue("MainWindow/y",this->y());
}

void Qtrading::closeEvent(QCloseEvent *event)
{
    SaveSizeAndPosition();
    qDebug() << "Qtrading close enter";
    event->accept();
    mainwindowhistory->close();
    mainwindowlog->CloseLog();
    mainwindowlog->close();


    qDebug() << "Qtrading close exit";
}



void Qtrading::on_actionConnect_triggered()
{
    dialogConnection->show();
}


void Qtrading::on_actionLog_toggled(bool arg1)
{
    if (arg1 == true)
        mainwindowlog->show();
    else
        mainwindowlog->hide();
    GUI_Settings->setValue("GUI/MainWindowLogVisible",arg1);
}

void Qtrading::on_actionHistory_toggled(bool arg1)
{
    if (arg1 == true)
        mainwindowhistory->show();
    else
        mainwindowhistory->hide();

    GUI_Settings->setValue("GUI/MainWindowHistoryVisible",arg1);
}

void Qtrading::on_actionLive_Graph_toggled(bool arg1)
{

    GUI_Settings->setValue("GUI/MainWindowGraphLiveVisible",arg1);
}

void Qtrading::on_actionConnect_to_REAL_triggered()
{
    connectionRequest("REAL");
}

void Qtrading::on_actionConnect_to_DEMO_triggered()
{
    connectionRequest("DEMO");
}

void Qtrading::connectionRequest(QString Type)
{
    ig_AccountType = Type;

    ui->lineEdit_AccountType->setText(ig_AccountType);
    struct StructProxy Proxy = dialogConnection->Getproxy();

    struct StructAccountInfo CurrentAccount;

    if (Type == "REAL")
        CurrentAccount= dialogConnection->GetAccountRealInfo();
    else
        CurrentAccount= dialogConnection->GetAccountDemoInfo();


    mainwindowhistory->ClearHistory();
    dialogConnection->SetAccountType(Type);
    igls->Connection(Proxy,CurrentAccount);

}

void Qtrading::AccountInfoUpdate(QString CashAvailable,QString balance ,QString profitLoss ,QString AccountID)
{
    ui->lineEdit_AccountCashAvailable->setText(CashAvailable);
    ui->lineEdit_AccountBalance->setText(balance);
    ui->lineEdit_AccountProfitLoss->setText(profitLoss);
    ui->lineEdit_AccountID->setText((AccountID));
}




void Qtrading::on_actionTrading_toggled(bool arg1)
{

    GUI_Settings->setValue("GUI/MainWindowTradingLiveVisible",arg1);
}


void Qtrading::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

    qDebug() << "event Type" <<event->type();
    if(event->type() == QEvent::WindowStateChange)
    if(isMinimized())
    {

        if(mainwindowhistory->isVisible())
            mainwindowhistory->showMinimized();
        if(mainwindowlog->isVisible())
            mainwindowlog->showMinimized();
    }

}
