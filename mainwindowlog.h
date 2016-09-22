#ifndef MAINWINDOWLOG_H
#define MAINWINDOWLOG_H

#include <QMainWindow>
#include "QTime"
#include "common.h"
#include "QDebug"
#include "QSettings"
#include "QFile"

namespace Ui {
class MainWindowLog;
}

class MainWindowLog : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowLog(QSettings *GUI_Settings,QWidget *parent = 0);
    ~MainWindowLog();

public slots:
    void Write_DEBUG(QString log);
    void write_LOG(int debug_level, QTime timestamp, QString log);
    void SaveSizeAndPosition();
    void CreateLog();
    void CloseLog();
private slots:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindowLog *ui;
    QSettings *GUI_Settings;
    QFile log_file;
signals :
    void MainWindowClosed(bool);
};

#endif // MAINWINDOWLOG_H
