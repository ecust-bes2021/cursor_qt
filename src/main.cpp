#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFile>
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

    const QString stylePath =
        QCoreApplication::applicationDirPath() + "/resources/styles/style.qss";
    QFile styleFile(stylePath);
    if (styleFile.exists() &&
        styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        a.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
        LOG_INFO() << "加载样式表:" << stylePath;
    }
    else
    {
        if (!styleFile.exists())
        {
            LOG_WARNING() << "未找到样式表:" << stylePath;
        }
        else
        {
            LOG_WARNING() << "无法打开样式表:" << stylePath;
        }
    }

    MainWindow w;
    w.show();

    LOG_INFO() << "主窗口已显示";

    int result = a.exec();

    LOG_INFO() << "应用程序退出，返回码:" << result;
    LOG_INFO() << "========================================";

    return result;
}
