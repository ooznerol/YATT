#ifndef MAINWINDOWTRADINGLIVE_H
#define MAINWINDOWTRADINGLIVE_H

#include <QMainWindow>
#include "QSettings"
#include "QDateTime"
#include <QCloseEvent>

namespace Ui {
class MainWindowTradingLive;
}

class MainWindowTradingLive : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowTradingLive(QSettings *Settings,QWidget *parent = 0);
    ~MainWindowTradingLive();

public slots:
    void SaveSizeAndPosition();

    void UpdatePrice(QString PriceBuy, QString PriceSell);
    void EnableTrading(bool);
    void UpdatePositionLevel(QString Direction,double level,QDateTime TradeCreateDate);
    void Reset();
private:
    Ui::MainWindowTradingLive *ui;
    QSettings *GUI_Settings;

    double MaxPnL;
    double MinPnL;
    bool bGetOpenPositionStatus;
    double TradeLevel;
    QString TradeDirection;
    QDateTime TradeCreateDate;

signals:
    void MainWindowClosed(bool);
    void openPosition(QString Direction,QString Size,QString Stop,QString Limit);

private slots:
    void on_pushButtonBuy_clicked();
    void on_pushButtonSell_clicked();
    void OpenPosition(QString Direction);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOWTRADINGLIVE_H
