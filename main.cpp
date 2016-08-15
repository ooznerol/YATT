#include "qtrading.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qtrading w;
    w.show();

    return a.exec();
}
