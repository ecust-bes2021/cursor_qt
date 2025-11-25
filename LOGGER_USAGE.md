# Qt 日志系统使用说明（基于 logger.h/logger.cpp）

本文档说明当前项目中自带的日志系统设计思路、使用方法和常见问题。该日志系统基于 Qt，目标是：

- 像 Python 的 `logging` 一样简单易用（`LOG_INFO() << ...`）
- 同时输出到 **日志文件** 和 **控制台**
- 可在 **任意 Qt Widgets 项目** 中复用
- 不影响 Qt 的 **UI 显示** 和 **事件循环**

---

## 1. 日志系统整体结构

主要文件：

- `logger.h`
  - 日志级别枚举：`LogLevel`
  - 日志单例类：`Logger`
  - 便捷宏：`LOG_DEBUG/LOG_INFO/LOG_WARNING/LOG_ERROR/LOG_FATAL`
- `logger.cpp`
  - `Logger` 的具体实现
  - Qt 全局消息处理函数：`Logger::messageHandler`
- `main.cpp`
  - 在应用程序启动时初始化日志系统

典型日志输出格式：

```text
2025-11-24 20:32:15.123 [INFO]     程序启动
2025-11-24 20:32:15.125 [INFO]     主窗口已显示
2025-11-24 20:32:16.456 [WARNING]  配置文件不存在，使用默认配置
2025-11-24 20:32:18.012 [ERROR]    数据库连接失败
```

- 时间精度：毫秒（`yyyy-MM-dd hh:mm:ss.zzz`）
- 级别字段宽度对齐，便于肉眼扫日志

---

## 2. 在 main.cpp 中的初始化方式

当前项目的 `main.cpp` 中已经集成了日志初始化：

```cpp
#include "mainwindow.h"
#include "logger.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const QString logPath =
        QCoreApplication::applicationDirPath() + "/logs/app.log";
    Logger::init(logPath, LogLevel::DEBUG, true, true);

    MainWindow w;
    w.show();

    return a.exec();
}
```

`Logger::init` 参数含义：

```cpp
Logger::init(
    const QString &logFilePath,   // 日志文件路径
    LogLevel minLevel = LogLevel::DEBUG,  // 最小输出级别
    bool consoleOutput = true,           // 是否同时输出到控制台
    bool installQtHandler = true         // 是否接管 Qt 自带日志
);
```

- **logFilePath**：一般写成 `应用程序目录/logs/app.log`
- **minLevel**：小于该级别的日志会被忽略（常用：`DEBUG` 或 `INFO`）
- **consoleOutput**：
  - `true`：写文件 + 写控制台（stdout/stderr）
  - `false`：只写文件
- **installQtHandler**：
  - `true`：`qDebug()/qInfo()/qWarning()/qCritical()` 也会进入该日志系统
  - `false`：只记录 `LOG_xxx()` 宏产生的日志，不改动 Qt 的默认行为

> 如果不想改动 Qt 默认的控制台输出，只想额外写日志文件，可以把最后一个参数设为 `false`：
>
> ```cpp
> Logger::init(logPath, LogLevel::DEBUG, true, false);
> ```

---

## 3. 在项目任意位置输出日志

在任意 `.cpp` 文件中：

1. 头文件中包含：

```cpp
#include "logger.h"
```

2. 使用类似 Python logging 的方式输出：

```cpp
void someFunction()
{
    LOG_DEBUG()   << "调试信息，变量值=" << value;
    LOG_INFO()    << "普通业务流程";
    LOG_WARNING() << "配置缺失，使用默认值";
    LOG_ERROR()   << "发生错误，错误码=" << errorCode;
}
```

这些日志会被统一格式化为：

```text
2025-11-24 20:32:15.125 [DEBUG]   调试信息，变量值= 42
2025-11-24 20:32:15.126 [INFO]    普通业务流程
2025-11-24 20:32:15.127 [WARNING] 配置缺失，使用默认值
2025-11-24 20:32:15.128 [ERROR]   发生错误，错误码= 1001
```

注意：

- `LOG_xxx()` 的用法和 `qDebug() << ...;` 非常类似，都是基于 `<<` 拼接
- 一条日志就是一条 C++ 语句，语句结束（临时对象析构）时真正写入

---

## 4. 捕获 Qt 自带日志（可选）

当 `installQtHandler = true` 时，下面这些 Qt 日志宏也会进入同一套日志系统：

```cpp
qDebug()    << "Qt 调试信息";
qInfo()     << "Qt 一般信息";
qWarning()  << "Qt 警告信息";
qCritical() << "Qt 严重错误";
// qFatal()  << "致命错误";  // 会终止程序
```

映射关系：

- `QtDebugMsg`    → `LogLevel::DEBUG`
- `QtInfoMsg`     → `LogLevel::INFO`
- `QtWarningMsg`  → `LogLevel::WARNING`
- `QtCriticalMsg` → `LogLevel::ERROR`
- `QtFatalMsg`    → `LogLevel::FATAL`（写完日志后调用 `std::abort()`）

这样可以：

- 同时看到你自己的 `LOG_xxx()` 日志
- 也能记录 Qt 内部的各种信息（例如控件警告、布局问题等）

如果不希望接管 Qt 的日志，只记录自定义日志，把 `installQtHandler` 设为 `false` 即可。

---

## 5. 日志级别与过滤

当前定义的级别从低到高依次为：

```cpp
DEBUG < INFO < WARNING < ERROR < FATAL
```

- `Logger::init(logPath, LogLevel::DEBUG, ...)`：
  - 记录所有级别（开发调试阶段常用）
- `Logger::init(logPath, LogLevel::INFO, ...)`：
  - 忽略 `DEBUG`，只记录 `INFO` 及以上（线上环境可用）

运行时也可以动态调整：

```cpp
Logger::setLevel(LogLevel::INFO);   // 之后 DEBUG 日志会被过滤掉
```

---

## 6. 线程安全与性能

- 内部使用 `QMutex` + `QMutexLocker` 保护日志写入，**多线程安全**
- 每次写日志会：
  - 构造一行字符串
  - 追加写入文件
  - 可选写入控制台
- 对于普通桌面应用来说，这点开销非常可控，通常不会成为瓶颈
- 如果日志量非常大（例如高频循环中反复打印），建议：
  - 调高最小日志级别（比如只保留 `INFO` 及以上）
  - 减少循环中的日志条数

---

## 7. 在其他项目中复用

要在新的 Qt Widgets 项目中使用这套日志系统：

1. 复制文件：
   - 把当前项目中的 `logger.h`、`logger.cpp` 复制到新项目源代码目录

2. 在新项目的 CMakeLists.txt 中加入：

   ```cmake
   set(PROJECT_SOURCES
       main.cpp
       ...
       logger.cpp
       logger.h
   )
   ```

3. 在 `main.cpp` 中初始化：

   ```cpp
   #include "logger.h"

   int main(int argc, char *argv[])
   {
       QApplication a(argc, argv);

       const QString logPath =
           QCoreApplication::applicationDirPath() + "/logs/app.log";
       Logger::init(logPath, LogLevel::DEBUG, true, true);

       ...
       return a.exec();
   }
   ```

4. 在其他源码中按需 `#include "logger.h"`，然后使用 `LOG_INFO()` 等宏即可。

---

## 8. 常见问题（FAQ）

### 8.1 窗口不显示，会不会是日志系统导致的？

当前实现中已经特别避免了：

- 在 `Logger::init()` 里**不会在持锁状态下再次写日志**，避免自锁死锁
- `messageHandler` 不调用 Qt 自带日志宏，避免递归
- 日志代码不创建线程、不操作 UI，只做简单文件/控制台输出

如果出现“进程在后台但窗口不显示”的情况，更可能是：

- 主线程在其他地方被阻塞（例如耗时计算、死循环）
- `MainWindow` 创建失败或 `show()` 没被调用

可以在 `main.cpp` 中逐步加/删 `LOG_INFO()` 来定位问题点。

### 8.2 想临时清空日志文件怎么办？

可以在需要的地方调用：

```cpp
Logger::instance().clear();
```

这会把当前日志文件截断为 0 字节。

---

如果以后对日志格式、输出策略有新的需求（例如按天分文件、异步写日志、只输出到文件不显示在控制台等），可以在此文件基础上逐步扩展。
