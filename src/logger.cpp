#include "logger.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <cstdio>

Logger::Logger()
    : m_logFile(),
      m_mutex(),
      m_logFilePath(),
      m_minLevel(LogLevel::DEBUG),
      m_consoleOutput(true),
      m_installedHandler(false)
{
}

Logger::~Logger()
{
    if (m_installedHandler) {
        qInstallMessageHandler(nullptr);
    }

    if (m_logFile.isOpen()) {
        QMutexLocker locker(&m_mutex);
        writeUnlocked(LogLevel::INFO,
                      QStringLiteral("========== 日志系统关闭 =========="));
        m_logFile.close();
    }
}

Logger &Logger::instance()
{
    static Logger s_instance;
    return s_instance;
}

void Logger::init(const QString &logFilePath,
                  LogLevel minLevel,
                  bool consoleOutput,
                  bool installQtHandler)
{
    Logger &logger = instance();

    {
        QMutexLocker locker(&logger.m_mutex);

        logger.m_logFilePath = logFilePath;
        logger.m_minLevel = minLevel;
        logger.m_consoleOutput = consoleOutput;

        if (logger.m_logFile.isOpen()) {
            logger.m_logFile.close();
        }

        QFileInfo info(logFilePath);
        if (!info.absolutePath().isEmpty()) {
            QDir().mkpath(info.absolutePath());
        }

        logger.m_logFile.setFileName(logFilePath);
        logger.m_logFile.open(QIODevice::WriteOnly |
                              QIODevice::Append |
                              QIODevice::Text);
    } 

    if (installQtHandler) {
        qInstallMessageHandler(Logger::messageHandler);
        logger.m_installedHandler = true;
    }

    logger.write(LogLevel::INFO,
                 QStringLiteral("========== 日志系统初始化 =========="));
}

void Logger::setLevel(LogLevel level)
{
    Logger &logger = instance();
    QMutexLocker locker(&logger.m_mutex);
    logger.m_minLevel = level;
}

void Logger::write(LogLevel level, const QString &msg)
{
    Logger &logger = instance();
    QMutexLocker locker(&logger.m_mutex);
    logger.writeUnlocked(level, msg);
}

void Logger::clear()
{
    Logger &logger = instance();
    QMutexLocker locker(&logger.m_mutex);

    if (!logger.m_logFile.isOpen()) {
        return;
    }
    logger.m_logFile.resize(0);
}

QString Logger::levelToString(LogLevel level) const
{
    switch (level) {
    case LogLevel::DEBUG:   return QStringLiteral("[DEBUG]   ");
    case LogLevel::INFO:    return QStringLiteral("[INFO]    ");
    case LogLevel::WARNING: return QStringLiteral("[WARNING] ");
    case LogLevel::ERROR:   return QStringLiteral("[ERROR]   ");
    case LogLevel::FATAL:   return QStringLiteral("[FATAL]   ");
    }
    return QStringLiteral("[UNKNOWN] ");
}

void Logger::writeUnlocked(LogLevel level, const QString &msg)
{
    if (level < m_minLevel) {
        return;
    }

    const QString ts = QDateTime::currentDateTime().toString(
                QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz"));
    const QString line = QStringLiteral("%1 %2 %3")
                             .arg(ts, levelToString(level), msg);

    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << line << '\n';
        out.flush();
    }

    if (m_consoleOutput) {
        const QByteArray local = line.toLocal8Bit();
        std::FILE *out = (level >= LogLevel::ERROR) ? stderr : stdout;
        std::fprintf(out, "%s\n", local.constData());
        std::fflush(out);
    }

    if (level == LogLevel::FATAL) {
        if (m_logFile.isOpen()) {
            m_logFile.flush();
        }
        std::abort();
    }
}

void Logger::messageHandler(QtMsgType type,
                            const QMessageLogContext &/*context*/,
                            const QString &msg)
{
    LogLevel level = LogLevel::INFO;
    switch (type) {
    case QtDebugMsg:    level = LogLevel::DEBUG;   break;
    case QtInfoMsg:     level = LogLevel::INFO;    break;
    case QtWarningMsg:  level = LogLevel::WARNING; break;
    case QtCriticalMsg: level = LogLevel::ERROR;   break;
    case QtFatalMsg:    level = LogLevel::FATAL;   break;
    }

    Logger::instance().write(level, msg);
}

LogStream::LogStream(LogLevel level)
    : m_level(level),
      m_buffer(),
      m_stream(&m_buffer, QIODevice::WriteOnly)
{
}

LogStream::~LogStream()
{
    Logger::instance().write(m_level, m_buffer.trimmed());
}

