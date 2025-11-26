#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QMutex>
#include <QTextStream>
#include <QtGlobal>

// 日志级别

enum class LogLevel {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger
{
public:
    static Logger &instance();

    static void init(const QString &logFilePath,
                     LogLevel minLevel = LogLevel::DEBUG,
                     bool consoleOutput = true,
                     bool installQtHandler = true);

    static void setLevel(LogLevel level);

    // 日志级别位置
    void write(LogLevel level, const QString &msg);

    // 清理日志文件
    void clear();

    QString logFilePath() const { return m_logFilePath; }

private:
    Logger();
    ~Logger();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);

    // 内部工具（请确保在 m_mutex 加锁状态下调用）
    void writeUnlocked(LogLevel level, const QString &msg);

    QString levelToString(LogLevel level) const;

    QFile m_logFile;
    mutable QMutex m_mutex;
    QString m_logFilePath;
    LogLevel m_minLevel;
    bool m_consoleOutput;
    bool m_installedHandler;
};

// 简单的日志流类
class LogStream
{
public:
    explicit LogStream(LogLevel level);
    ~LogStream();

    template<typename T>
    LogStream &operator<<(const T &value)
    {
        m_stream << value;
        return *this;
    }

private:
    LogLevel m_level;
    QString m_buffer;
    QTextStream m_stream;
};

// 日志实现，用法类似于 Python logging
#define LOG_DEBUG()    LogStream(LogLevel::DEBUG)
#define LOG_INFO()     LogStream(LogLevel::INFO)
#define LOG_WARNING()  LogStream(LogLevel::WARNING)
#define LOG_ERROR()    LogStream(LogLevel::ERROR)
#define LOG_FATAL()    LogStream(LogLevel::FATAL)

#endif // LOGGER_H

