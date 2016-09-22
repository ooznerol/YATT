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


#include "mainwindowhistory.h"
#include "ui_mainwindowhistory.h"

MainWindowHistory::MainWindowHistory(QSettings *Settings,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowHistory)
{
    ui->setupUi(this);

    GUI_Settings = Settings;
    qDebug() << "QSettings path= " << GUI_Settings->fileName();
    ui->dateEdit_StartDate->setDate(QDate::currentDate());
    ui->dateEdit_EndDate->setDate(QDate::currentDate());

    GUI_Settings->setValue("MainWindowHistory","toto");

    //Load Size and Position
    bool ok;
    int width = GUI_Settings->value("MainWindowHistory/width",this->size().width()).toUInt(&ok);
    int height = GUI_Settings->value("MainWindowHistory/height",this->size().height()).toUInt(&ok);
    int x = GUI_Settings->value("MainWindowHistory/x",this->x()).toUInt(&ok);
    int y = GUI_Settings->value("MainWindowHistory/y",this->y()).toUInt(&ok);

    this->move(QPoint(x,y));
    this->resize(QSize(width,height));

    //Model trades
    modeltrades = new modelTrades(this);
    ui->tableView->setModel(modeltrades);
    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(15);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //ChartHistory
    chartHistory = new QChart();
    chartHistory->legend()->setVisible(false);
    QChartView *chartView = new QChartView(chartHistory);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout_chart->addWidget(chartView);

    ui->radioButton_TimeGraph->setChecked(GUI_Settings->value("TimeGraph",false).toBool());
    ui->radioButton_TradeGraph->setChecked(GUI_Settings->value("TradeGraph",true).toBool());


    //Disable button
    ui->pushButton_Get->setEnabled(false);
    ui->dateEdit_StartDate->setEnabled(false);
    ui->dateEdit_EndDate->setEnabled(false);
}

MainWindowHistory::~MainWindowHistory()
{
    delete ui;
}


void MainWindowHistory::closeEvent(QCloseEvent *event)
{
    event->accept();
    SaveSizeAndPosition();
    qDebug() << "MainWindowHistory MainWindowClosed";
    if (event->spontaneous())
        emit MainWindowClosed(false);
}

void MainWindowHistory::SaveSizeAndPosition()
{
    GUI_Settings->setValue("MainWindowHistory/width",this->size().width());
    GUI_Settings->setValue("MainWindowHistory/height",this->size().height());
    GUI_Settings->setValue("MainWindowHistory/x",this->x());
    GUI_Settings->setValue("MainWindowHistory/y",this->y());
}


void MainWindowHistory::updateAccoutnInfo(QString CashAvailable,QString balance,QString profitLoss,QString AccountID)
{
    ui->lineEdit_AccountCashAvailable->setText(CashAvailable);
    ui->lineEdit_AccountBalance->setText(balance);
    ui->lineEdit_AccountProfitLoss->setText(profitLoss);
    ui->lineEdit_AccountID->setText((AccountID));


}
void MainWindowHistory::EnableButton(bool state)
{
    ui->pushButton_Get->setEnabled(state);
    ui->dateEdit_StartDate->setEnabled(state);
    ui->dateEdit_EndDate->setEnabled(state);
}
void MainWindowHistory::updateTradesInfo(/*int numberOftradesWon,int numberOftradesLost,int  numberOftradesFlat,int numberOfPointsLost,int numberOfPointsWon*/ )
{
    ui->spinBox_TradesWon->setValue(numberOftradesWon);
    ui->spinBox_TradesLost->setValue(numberOftradesLost);
    ui->spinBox_TradesFlat->setValue(numberOftradesFlat);


    ui->doubleSpinBox_PointsLost->setValue(numberOfPointsLost);
    ui->doubleSpinBox_PointsWon->setValue(numberOfPointsWon);


    double TotalPoint = numberOfPointsWon + numberOfPointsLost;
    ui->doubleSpinBox_PointsTotal->setValue(TotalPoint);
    QPalette palette = ui->doubleSpinBox_PointsTotal->palette();
    if (TotalPoint > 0)
        palette.setColor(QPalette::Base,Qt::green);
    else if (TotalPoint < 0)
        palette.setColor(QPalette::Base,Qt::red);
    else
        palette.setColor(QPalette::Base,Qt::gray);
    ui->doubleSpinBox_PointsTotal->setPalette(palette);

    ui->tableView->resizeColumnsToContents();

    if (ui->radioButton_TimeGraph->isChecked())
        on_radioButton_TimeGraph_clicked(true);
    else
        on_radioButton_TradeGraph_clicked(true);

    emit write_LOG(LOG_SUCCESS,QTime::currentTime(),"Get " + QString::number(ListOfTransaction.size()) + " transactions");
}


void MainWindowHistory::ClearHistory()
{
    modeltrades->Clear();
    chartHistory->removeAllSeries();

    ui->spinBox_TradesFlat->clear();
    ui->spinBox_TradesWon->clear();
    ui->spinBox_TradesLost->clear();

    ui->doubleSpinBox_PointsLost->clear();
    ui->doubleSpinBox_PointsWon->clear();
    ui->doubleSpinBox_PointsTotal->clear();

    ui->lineEdit_AccountBalance->clear();
    ui->lineEdit_AccountCashAvailable->clear();
    ui->lineEdit_AccountID->clear();
    ui->lineEdit_AccountProfitLoss->clear();

    QPalette palette = ui->doubleSpinBox_PointsTotal->palette();
    palette.setColor(QPalette::Base,Qt::transparent);
    ui->doubleSpinBox_PointsTotal->setPalette(palette);


    ListOfTransaction.clear();

    numberOftradesWon = 0;
    numberOftradesLost = 0;
    numberOftradesFlat = 0;

    numberOfPointsLost = 0 ;
    numberOfPointsTotal = 0 ;
    numberOfPointsWon = 0;
}


QString MainWindowHistory::StartDate()
{
    return ui->dateEdit_StartDate->date().toString("yyyy-MM-dd");
}

QString MainWindowHistory::EndDate()
{
    return ui->dateEdit_EndDate->date().toString("yyyy-MM-dd");
}

void MainWindowHistory::on_pushButton_Get_clicked()
{
    ClearHistory();
    emit GetListOfTrades(ui->dateEdit_StartDate->dateTime(),ui->dateEdit_EndDate->dateTime());
}
void MainWindowHistory::ParseTransactionList(QJsonArray jsonArray)
{

     bool ok;
     foreach (const QJsonValue & value, jsonArray)
     {

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
    }
}



void MainWindowHistory::on_radioButton_TradeGraph_clicked(bool checked)
{
    if (checked == true)
    {
        chartHistory->removeAllSeries();

        QLineSeries *seriesTrades = new QLineSeries();
        QLineSeries *seriesZero = new QLineSeries();
        QScatterSeries *seriesPoint = new QScatterSeries();
        seriesPoint->setMarkerSize(5.0);

        seriesTrades->append(0,0);
        seriesZero->append(0,0);
        seriesPoint->append(0,0);

        float sum = 0;
        int index = 1 ;

        for (int i = ListOfTransaction.size(); i > 0 ; i--)
        {
            sum += ListOfTransaction.at(i-1).deltaPoint;
            seriesTrades->append(index,sum);
            seriesZero->append(index,0);
            seriesPoint->append(index,sum);
            index++;
        }

        qDebug() << "seriesTrades count =" << seriesTrades->count();

        chartHistory->addSeries(seriesTrades);
        chartHistory->addSeries(seriesZero);
        chartHistory->addSeries(seriesPoint);

        chartHistory->createDefaultAxes();


        chartHistory->removeAxis(chartHistory->axisX());
        QValueAxis *axisX = new QValueAxis;
        axisX->setTickCount(seriesTrades->count()-1);
        chartHistory->addAxis(axisX, Qt::AlignBottom);
        axisX->setTitleText("trades");
        axisX->setLabelFormat("%d");
        seriesPoint->attachAxis(axisX);
    }
    GUI_Settings->setValue("TradeGraph",checked);
}

void MainWindowHistory::on_radioButton_TimeGraph_clicked(bool checked)
{

    if (checked == true)
    {
        chartHistory->removeAllSeries();

        QLineSeries *seriesTradesDate = new QLineSeries();
        QLineSeries *seriesZeroForDate = new QLineSeries();


        float sum = 0;
        int index = 1 ;
        for (int i = ListOfTransaction.size(); i > 0 ; i--)
        {
            sum += ListOfTransaction.at(i-1).deltaPoint;
            //seriesPoint->append(index,sum);

            QString DateUTC = ListOfTransaction.at(i-1).dateUTC;

            QDateTime momentInTime = QDateTime::fromString(DateUTC,"yyyy-MM-ddThh:mm:ss");

            seriesTradesDate->append(momentInTime.toMSecsSinceEpoch(),sum);
            seriesZeroForDate->append(momentInTime.toMSecsSinceEpoch(),0);

            index++;

        }



        chartHistory->addSeries(seriesTradesDate);
        chartHistory->addSeries(seriesZeroForDate);

        chartHistory->createDefaultAxes();

        chartHistory->removeAxis(chartHistory->axisX());
        chartHistory->axisY()->setTitleText("points");

        QDateTimeAxis *axisX = new QDateTimeAxis;

        axisX->setFormat("dd-MM-yyyy hh:mm:ss");
        axisX->setTitleText("Trade Date");
        chartHistory->addAxis(axisX, Qt::AlignBottom);
        seriesTradesDate->attachAxis(axisX);
    }
    GUI_Settings->setValue("TimeGraph",checked);

}
