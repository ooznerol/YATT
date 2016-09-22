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


#include "mainwindowtradinglive.h"
#include "ui_mainwindowtradinglive.h"

MainWindowTradingLive::MainWindowTradingLive(QSettings *Settings,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowTradingLive)
{
    ui->setupUi(this);
    GUI_Settings = Settings;

    ui->pushButtonBuy->setEnabled(false);
    ui->pushButtonSell->setEnabled(false);
    ui->pushButtonClose->setEnabled(false);

    bool ok;
    ui->doubleSpinBoxLimit->setValue(GUI_Settings->value("Limit",0).toDouble(&ok));
    ui->doubleSpinBoxStop->setValue(GUI_Settings->value("Stop",0).toDouble(&ok));
    ui->doubleSpinBoxSize->setValue(GUI_Settings->value("Size",1).toDouble(&ok));



    //Load Size and Position

    int width = GUI_Settings->value("MainWindowTradingLive/width",this->size().width()).toUInt(&ok);
    int height = GUI_Settings->value("MainWindowTradingLive/height",this->size().height()).toUInt(&ok);
    int x = GUI_Settings->value("MainWindowTradingLive/x",this->x()).toUInt(&ok);
    int y = GUI_Settings->value("MainWindowTradingLive/y",this->y()).toUInt(&ok);

    this->move(QPoint(x,y));
    this->resize(QSize(width,height));
}

MainWindowTradingLive::~MainWindowTradingLive()
{
    delete ui;
}

void MainWindowTradingLive::closeEvent(QCloseEvent *event)
{
    event->accept();
    SaveSizeAndPosition();
    if (event->spontaneous())
        emit MainWindowClosed(false);
}

void MainWindowTradingLive::SaveSizeAndPosition()
{
//    qDebug() << "MainWindowLog save position enter";
    GUI_Settings->setValue("MainWindowLog/width",this->size().width());
    GUI_Settings->setValue("MainWindowLog/height",this->size().height());
    GUI_Settings->setValue("MainWindowLog/x",this->x());
    GUI_Settings->setValue("MainWindowLog/y",this->y());
//    qDebug() << "MainWindowLog save position exit";
}





void MainWindowTradingLive::UpdatePositionLevel(QString Direction,double level,QDateTime Date )
{
    ui->lineEditCurrentTradeLevel->setText(QString::number(level));
    TradeCreateDate = Date;
    TradeLevel = level;
    TradeDirection = Direction;
    bGetOpenPositionStatus = true;

}

void MainWindowTradingLive::Reset()
{
    bGetOpenPositionStatus = false;

            MaxPnL = 0;
    MinPnL = 0;

    ui->doubleSpinBoxTradeStatus->clear();
    QPalette palette = ui->doubleSpinBoxTradeStatus->palette();
    palette.setColor(QPalette::Base,Qt::gray);
    ui->doubleSpinBoxTradeStatus->setPalette(palette);

    ui->doubleSpinBoxTradeStatusMax->clear();
    ui->doubleSpinBoxTradeStatusMin->clear();

    ui->lineEditCurrentTradeLevel->clear();
    ui->lineEditCurrentTradeTime->clear();


}

void MainWindowTradingLive::EnableTrading(bool enable)
{
    ui->pushButtonBuy->setEnabled(enable);
    ui->pushButtonSell->setEnabled(enable);
}

void MainWindowTradingLive::UpdatePrice(QString PriceBuy,QString PriceSell)
{
    ui->pushButtonBuy->setText(PriceBuy);
    ui->pushButtonSell->setText(PriceSell);

    bool ok;
    ui->doubleSpinBoxSpread->setValue(PriceBuy.toDouble(&ok) - PriceSell.toDouble(&ok));

    //Display min and max PnL
    ui->doubleSpinBoxTradeStatusMax->setValue(MaxPnL);
    if (MaxPnL > 0)
    {
        QPalette palette = ui->doubleSpinBoxTradeStatusMax->palette();
        palette.setColor(QPalette::Text,Qt::green);
        ui->doubleSpinBoxTradeStatusMax->setPalette(palette);
    }

    ui->doubleSpinBoxTradeStatusMin->setValue(MinPnL);
    if (MinPnL < 0)
    {
        QPalette palette = ui->doubleSpinBoxTradeStatusMin->palette();
        palette.setColor(QPalette::Text,Qt::red);
        ui->doubleSpinBoxTradeStatusMin->setPalette(palette);
    }

    double TradePnL;
    double PriceMarket;






//                    if (tick == 0)
//                        MaxPriceMarket = MinPriceMarket = CurrentLiveTransaction.level;
    if (TradeDirection == "BUY")
    {
        PriceMarket = PriceSell.toDouble(&ok);
        TradePnL= PriceMarket - TradeLevel ;

    }
    else
    {
        PriceMarket = PriceBuy.toDouble(&ok);
        TradePnL= TradeLevel - PriceMarket;
    }

    //Display current PnL
    ui->doubleSpinBoxTradeStatus->setValue(TradePnL);
    QPalette ButtonPalette = ui->doubleSpinBoxTradeStatus->palette();

    if (TradePnL > 0)
    {
        ButtonPalette.setColor(QPalette::Base,Qt::green);
        ui->doubleSpinBoxTradeStatus->setPalette(ButtonPalette);
    }
    else
    {
        ButtonPalette.setColor(QPalette::Base,Qt::red);
        ui->doubleSpinBoxTradeStatus->setPalette(ButtonPalette);
    }

    //calculate min and max PnL
    if (TradePnL > 0)
    {
        if (TradePnL > MaxPnL)
        MaxPnL = TradePnL;
    }
    if (TradePnL < 0)
    {
        if (TradePnL < MinPnL)
        MinPnL = TradePnL;
    }

    //UPdate Time
    uint Time0 = TradeCreateDate.toTime_t();
    uint Time1 = QDateTime::currentDateTime().toTime_t();
    uint DeltaTime = Time1 - Time0;
    QDateTime Delta = QDateTime::fromTime_t(DeltaTime);
    ui->lineEditCurrentTradeTime->setText(Delta.time().toString("hh:mm:ss"));


}

void MainWindowTradingLive::on_pushButtonBuy_clicked()
{
    OpenPosition("BUY");
}

void MainWindowTradingLive::on_pushButtonSell_clicked()
{
     OpenPosition("SELL");
}

void MainWindowTradingLive::OpenPosition(QString Direction)
{

    QString Size = QString::number(ui->doubleSpinBoxSize->value());
    QString Stop = QString::number(ui->doubleSpinBoxStop->value());
    QString Limit = QString::number(ui->doubleSpinBoxLimit->value());

    GUI_Settings->setValue("Size",ui->doubleSpinBoxSize->value());
    GUI_Settings->setValue("Stop",ui->doubleSpinBoxStop->value());
    GUI_Settings->setValue("Limit",ui->doubleSpinBoxLimit->value());

    emit openPosition(Direction,Size,Stop,Limit);
}
