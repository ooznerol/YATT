#ifndef MAINWINDOWHISTORY_H
#define MAINWINDOWHISTORY_H

#include <QMainWindow>
#include "common.h"
#include "modeltrades.h"
#include "QSettings"

#include <QtCharts>
using namespace QtCharts;

namespace Ui {
class MainWindowHistory;
}

class MainWindowHistory : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowHistory(QSettings *GUI_Settings,QWidget *parent = 0);
    ~MainWindowHistory();

public slots:
    void updateAccoutnInfo(QString CashAvailable, QString balance, QString profitLoss, QString AccountID);
    void EnableButton(bool state);
    void updateTradesInfo(/*int numberOftradesWon, int numberOftradesLost, int numberOftradesFlat, int numberOfPointsLost, int numberOfPointsWon*/);
    void ParseTransactionList(QJsonArray jsonArray);
    void ClearHistory();
    QString StartDate();
    QString EndDate();
    void SaveSizeAndPosition();
private slots:
    void on_pushButton_Get_clicked();

    void on_radioButton_TimeGraph_clicked(bool checked);
    void on_radioButton_TradeGraph_clicked(bool checked);
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindowHistory *ui;
    modelTrades* modeltrades;
    QChart *chartHistory;

    int numberOftradesWon ;
    int numberOftradesLost;
    int numberOftradesFlat ;

    double numberOfPointsWon ;
    double numberOfPointsLost;
    double numberOfPointsTotal;

    QSettings *GUI_Settings;

    QList <TransactionStruct> ListOfTransaction;

signals :
    void GetListOfTrades(QDateTime,QDateTime);
    void MainWindowClosed(bool);
    void write_LOG(int,QTime,QString);

};

#endif // MAINWINDOWHISTORY_H
