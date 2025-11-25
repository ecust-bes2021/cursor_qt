#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化日志系统（必须在使用任何日志宏之前调用）
    const QString logPath =
        QCoreApplication::applicationDirPath() + "/logs/app.log";
    Logger::init(logPath, LogLevel::DEBUG, true, true);

    LOG_INFO() << "========================================";
    LOG_INFO() << "应用程序启动";
    LOG_DEBUG() << "应用程序路径:" << QCoreApplication::applicationDirPath();

    MainWindow w;
    w.show();

    LOG_INFO() << "主窗口已显示";

    int result = a.exec();

    LOG_INFO() << "应用程序退出，返回码:" << result;
    LOG_INFO() << "========================================";

    return result;
}
