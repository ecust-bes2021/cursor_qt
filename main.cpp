#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const QString logPath =
        QCoreApplication::applicationDirPath() + "/logs/app.log";

    MainWindow w;
    w.show();

    return a.exec();
}
