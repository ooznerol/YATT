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


#include "mainwindowgraphlive.h"
#include "ui_mainwindowgraphlive.h"

MainWindowGraphLive::MainWindowGraphLive(QSettings *Settings,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowGraphLive)
{
    ui->setupUi(this);

    GUI_Settings = Settings;

    //Load Size and Position
    bool ok;
    int width = GUI_Settings->value("MainWindowGraphLive/width",this->size().width()).toUInt(&ok);
    int height = GUI_Settings->value("MainWindowGraphLive/height",this->size().height()).toUInt(&ok);
    int x = GUI_Settings->value("MainWindowGraphLive/x",this->x()).toUInt(&ok);
    int y = GUI_Settings->value("MainWindowGraphLive/y",this->y()).toUInt(&ok);

    this->move(QPoint(x,y));
    this->resize(QSize(width,height));

    //ChartLive
    chartLive = new QChart();
    chartLive->legend()->setVisible(false);
    QChartView *chartViewLive = new QChartView(chartLive);
    chartViewLive->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout_ChartLive->addWidget(chartViewLive);


    m_series = new QSplineSeries(this);
    QPen green(Qt::blue);
    green.setWidth(3);
    m_series->setPen(green);

    chartLive->addSeries(m_series);

    m_seriesZero = new QLineSeries(this);
    QPen blue(Qt::black);
    m_seriesZero->setPen(blue);
    chartLive->addSeries(m_seriesZero);

    chartLive->createDefaultAxes();
}


void MainWindowGraphLive::closeEvent(QCloseEvent *event)
{

    SaveSizeAndPosition();
    event->accept();
    if (event->spontaneous())
        emit MainWindowClosed(false);
}
void MainWindowGraphLive::SaveSizeAndPosition()
{
    GUI_Settings->setValue("MainWindowGraphLive/width",this->size().width());
    GUI_Settings->setValue("MainWindowGraphLive/height",this->size().height());
    GUI_Settings->setValue("MainWindowGraphLive/x",this->x());
    GUI_Settings->setValue("MainWindowGraphLive/y",this->y());
}

void MainWindowGraphLive::setTradeLevel(QString Direction,double TradeLevel)
{
    CurrentTradeDirection = Direction;
    CurrentTradeLevel = TradeLevel;
}

void MainWindowGraphLive::AppendValuetoGraph(quint32 tick,double PriceMarket)
{
    if (tick == 0)
        MaxPriceMarket = MinPriceMarket = CurrentTradeLevel;
    //calculate min and max Price
    if (PriceMarket > MaxPriceMarket)
        if (PriceMarket >CurrentTradeLevel)
            MaxPriceMarket = PriceMarket;
    if (PriceMarket < MinPriceMarket)
        if (PriceMarket < CurrentTradeLevel)
            MinPriceMarket = PriceMarket;


    chartLive->axisX()->setRange(0,tick);
    chartLive->axisY()->setRange(MinPriceMarket -1,MaxPriceMarket + 1);


    m_series->append(tick,PriceMarket);
    m_seriesZero->append(tick,CurrentTradeLevel);

}

void MainWindowGraphLive::ClearGraph()
{
    m_series->clear();
    m_seriesZero->clear();

}


MainWindowGraphLive::~MainWindowGraphLive()
{
    delete ui;
}
