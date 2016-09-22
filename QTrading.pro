#-------------------------------------------------
#
# Project created by QtCreator 2015-08-05T16:37:40
#
#-------------------------------------------------

QT       += core gui

QT += network
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YATT
TEMPLATE = app




SOURCES += main.cpp\
        qtrading.cpp \
    	modeltrades.cpp \
    	dialogconnection.cpp \
    mainwindowlog.cpp \
    mainwindowhistory.cpp \
    igls.cpp

#   // sleeper.cpp

HEADERS  += qtrading.h \
    sleeper.h \
    modeltrades.h \
    common.h \
    dialogconnection.h \
    mainwindowlog.h \
    mainwindowhistory.h \
    igls.h


FORMS    += qtrading.ui \
    dialogconnection.ui \
    mainwindowlog.ui \
    mainwindowhistory.ui \


RESOURCES += \
    image.qrc

DISTFILES += \
    Qtrading.rc

win32:RC_ICONS += finance.ico
#macx:ICON = $${PWD}/my_app.icns
macx:ICON = finance.icns


#######################################
#WINDEPLOYQT
#######################################
win32 {
    DEPLOY_COMMAND = windeployqt
}
macx {
    DEPLOY_COMMAND = macdeployqt
}
DEPLOY_TARGET = $$OUT_PWD

#Uncomment the following line to help debug the deploy command when running qmake
#warning($${DEPLOY_COMMAND} $$shell_quote($${DEPLOY_TARGET}/release))
QMAKE_POST_LINK += $${DEPLOY_COMMAND} $$shell_quote($${DEPLOY_TARGET}/release) &
