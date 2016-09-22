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


#include "mainwindowlog.h"
#include "ui_mainwindowlog.h"

MainWindowLog::MainWindowLog(QSettings *Settings,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowLog)
{
    ui->setupUi(this);
    GUI_Settings = Settings;

    //Load Size and Position
    bool ok;
    int width = GUI_Settings->value("MainWindowLog/width",this->size().width()).toUInt(&ok);
    int height = GUI_Settings->value("MainWindowLog/height",this->size().height()).toUInt(&ok);
    int x = GUI_Settings->value("MainWindowLog/x",this->x()).toUInt(&ok);
    int y = GUI_Settings->value("MainWindowLog/y",this->y()).toUInt(&ok);

    this->move(QPoint(x,y));
    this->resize(QSize(width,height));

}

MainWindowLog::~MainWindowLog()
{
    CloseLog();
    delete ui;
}

void MainWindowLog::closeEvent(QCloseEvent *event)
{
    event->accept();
    SaveSizeAndPosition();
    if (event->spontaneous())
        emit MainWindowClosed(false);
}

void MainWindowLog::SaveSizeAndPosition()
{
    GUI_Settings->setValue("MainWindowLog/width",this->size().width());
    GUI_Settings->setValue("MainWindowLog/height",this->size().height());
    GUI_Settings->setValue("MainWindowLog/x",this->x());
    GUI_Settings->setValue("MainWindowLog/y",this->y());
}

void MainWindowLog::CloseLog()
{
    if (log_file.isOpen())
    {
        log_file.close();
    }
}

void MainWindowLog::CreateLog()
{
    //create Log file
    QString application_dir = qApp->applicationDirPath();
    QString log_file_path = application_dir + "/YATT.log";


    log_file.setFileName(log_file_path);
    if (log_file.exists())
        log_file.remove();

    qDebug() << "Open/create log file " <<  log_file_path;

    bool log_is_open = log_file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);

    write_LOG(LOG_NORMAL,QTime::currentTime(),"log path : " + log_file_path );

    if (log_is_open)
        write_LOG(LOG_SUCCESS,QTime::currentTime(),"open/create log file SUCCESS" );
    else
        write_LOG(LOG_ERROR,QTime::currentTime(),"open/create log file FAILED" );

    //Get operating System
    QString OPerating_system;

    #ifdef Q_OS_LINUX
    OPerating_system = "Linux";
    #endif

    #ifdef Q_OS_MAC
    OPerating_system = "Mac";
    #endif

    #ifdef Q_OS_WIN32
    switch(QSysInfo::windowsVersion())
    {
      case QSysInfo::WV_2000: OPerating_system =  "Windows 2000";
      case QSysInfo::WV_XP: OPerating_system = "Windows XP";
      case QSysInfo::WV_VISTA: OPerating_system = "Windows Vista";
      case QSysInfo::WV_WINDOWS7:OPerating_system = "windows 7";
      default:
        break;
    }
    #ifdef Q_OS_WIN64
        OPerating_system += " 64bit";
    #else
        OPerating_system += " 32bit";
    #endif

    #endif
    write_LOG(LOG_NORMAL,QTime::currentTime(),"OS = " + OPerating_system);
}


void MainWindowLog::Write_DEBUG(QString log)
{
    ui->textEditDebug->append(log);
        QTextCursor c = ui->textEditDebug->textCursor();
        c.movePosition(QTextCursor::End);
        ui->textEditDebug->setTextCursor(c);
}

void MainWindowLog::write_LOG(int debug_level, QTime timestamp, QString log)
{
#if 1

    QString type_field;
    QString message;

    QPalette p = ui->plainTextEdit->palette();

    QTextCharFormat tf;
    tf = ui->plainTextEdit->currentCharFormat();


    QColor textColor;


//    if (DEBUG_LEVEL <= debug_level)
//    {
         if (debug_level & LOG_ERROR)
         {
             type_field = "ERROR\t";
             textColor = QColor(Qt::red);
         }
         else if (debug_level & LOG_SUCCESS)
         {
             type_field = "SUCCESS\t";
             textColor = QColor(Qt::green);
         }
         else if (debug_level & LOG_DEBUG)
         {
             type_field = "DEBUG\t";
             textColor = QColor(Qt::gray);
         }
         else if (debug_level & LOG_WARNING)
         {
             type_field = "WARNING\t";
             textColor = QColor(255, 175, 0);
         }
         else
         {
             type_field = "INFO\t";
             textColor = QColor(Qt::black);
         }
//    }

    tf.setForeground(QBrush(textColor));

    ui->plainTextEdit->setCurrentCharFormat(tf);


    message = timestamp.toString("hh:mm:ss.zzz") + " : " +  type_field + ":" + log/* +  "\r\n"*/;
    qDebug() << message;

    ui->plainTextEdit->appendPlainText(message);


    if (log_file.isOpen())
    {
        QTextStream out(&log_file);
        out << message << endl;
    }






    QTextCursor c = ui->textEditDebug->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEditDebug->setTextCursor(c);
#endif

}
