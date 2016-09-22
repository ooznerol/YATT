#ifndef MAINWINDOWGRAPHLIVE_H
#define MAINWINDOWGRAPHLIVE_H

#include <QMainWindow>
#include "common.h"
#include "QSettings"

#include <QtCharts>
using namespace QtCharts;

namespace Ui {
class MainWindowGraphLive;
}

class MainWindowGraphLive : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowGraphLive(QSettings *Settings,QWidget *parent = 0);
    ~MainWindowGraphLive();

public slots:
    void AppendValuetoGraph(quint32,double);
    void ClearGraph();
    void setTradeLevel(QString Dir,double TradeLevel);
    void SaveSizeAndPosition();
private slots:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindowGraphLive *ui;
    QChart *chartLive;

    QSplineSeries *m_series;
    QLineSeries *m_seriesZero;

    QStringList m_titles;
    QValueAxis *m_axis;

    double MaxPriceMarket;
    double MinPriceMarket;

    QString CurrentTradeDirection;
    double CurrentTradeLevel;
    QSettings *GUI_Settings;
signals :
    void MainWindowClosed(bool);
};

#endif // MAINWINDOWGRAPHLIVE_H
