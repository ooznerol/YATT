#ifndef QTRADING_H
#define QTRADING_H

#include <QMainWindow>
#include <QtNetwork>
#include <QUrlQuery>
#include "QSettings"
#include "common.h"

#include "dialogconnection.h"




#include "mainwindowlog.h"
#include "mainwindowhistory.h"
#include "mainwindowgraphlive.h"
#include "mainwindowtradinglive.h"
#include "igls.h"



namespace Ui {
class Qtrading;
}

class Qtrading : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qtrading(QWidget *parent = 0);
    ~Qtrading();



public slots:
    void AccountInfoUpdate(QString CashAvailable, QString balance, QString profitLoss, QString AccountID);
    void HideDialogConnection(bool status);

private slots:
    void on_actionConnect_triggered();

    void on_actionLog_toggled(bool arg1);
    void on_actionHistory_toggled(bool arg1);
    void on_actionLive_Graph_toggled(bool arg1);
    void on_actionTrading_toggled(bool arg1);

    void on_actionConnect_to_REAL_triggered();
    void on_actionConnect_to_DEMO_triggered();

    void connectionRequest(QString Type);

    void SaveSizeAndPosition();
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
private:
    Ui::Qtrading *ui;
    QSettings *GUI_Settings;

    DialogConnection *dialogConnection;

    QString ig_AccountType;


    QTimer *TimerGetOpenPosition;

    MainWindowLog *mainwindowlog;
    MainWindowHistory *mainwindowhistory;


    IGLS *igls;







};

#endif // QTRADING_H
