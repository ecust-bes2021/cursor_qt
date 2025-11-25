# cursor_qt

一个基于 Qt 6 的桌面应用程序，提供文件路径选择、配置管理和完善的日志系统。

## 📋 项目概述

cursor_qt 是一个 Qt C++ 桌面应用程序模板项目，展示了现代 Qt 应用开发的最佳实践。

### 主要特性

- ✅ **文件路径选择**：通过图形界面选择文件路径
- ✅ **配置管理**：基于 JSON 的配置文件加载和保存
- ✅ **日志系统**：完善的日志记录功能，支持文件和控制台双输出
- ✅ **跨平台**：基于 Qt 6，支持 Windows、Linux、macOS
- ✅ **CMake 构建**：现代化的构建系统，支持 Debug/Release 分离
- ✅ **绿色软件**：所有配置和数据文件位于程序目录，无需安装

---

## 🛠️ 技术栈

| 技术 | 版本/说明 |
|------|----------|
| **Qt** | 6.9.3 |
| **编译器** | MinGW 64-bit (GCC) |
| **构建系统** | CMake 3.16+ |
| **C++ 标准** | C++17 |
| **UI 框架** | Qt Widgets |

---

## 📁 项目结构

```
cursor_qt/
├── bin/                      # 可执行文件输出目录
│   ├── Debug/               # Debug 版本
│   │   ├── cursor_qt.exe   # 主程序
│   │   ├── config/         # 配置文件目录
│   │   ├── logs/           # 日志文件目录
│   │   └── *.dll           # Qt 依赖库
│   └── Release/            # Release 版本
├── build/                   # CMake 构建目录
├── docs/                    # 文档目录
│   ├── LOGGER_USAGE.md     # 日志系统使用说明
│   └── JSON_USAGE.md       # JSON 处理使用说明
├── main.cpp                 # 程序入口
├── mainwindow.h/cpp/ui      # 主窗口
├── logger.h/cpp             # 日志系统
├── config.json              # 配置文件模板
└── CMakeLists.txt           # CMake 配置文件
```

### 核心文件说明

| 文件 | 说明 |
|------|------|
| `main.cpp` | 程序入口，初始化 QApplication 和主窗口 |
| `mainwindow.h/cpp/ui` | 主窗口类，实现 UI 逻辑和配置管理 |
| `logger.h/cpp` | 日志系统，提供类似 Python logging 的接口 |
| `config.json` | 配置文件，存储应用程序设置 |
| `CMakeLists.txt` | CMake 构建配置 |

---

## ✨ 功能说明

### 1. 文件路径选择

- 提供三个文件路径输入框
- 点击 "Browser" 按钮打开文件选择对话框
- 支持选择任意类型的文件
- 选择的路径会自动保存到配置文件

### 2. 配置管理

- **自动加载**：程序启动时从 `bin/Debug/config/config.json` 加载配置
- **自动保存**：程序退出时自动保存当前配置
- **配置项**：
  - `filePath1`：第一个文件路径
  - `filePath2`：第二个文件路径
  - `filePath3`：第三个文件路径

### 3. 日志系统

- **双输出**：同时输出到控制台和日志文件
- **日志级别**：DEBUG、INFO、WARNING、ERROR、FATAL
- **时间戳**：毫秒级精度
- **线程安全**：支持多线程环境
- **Qt 集成**：可接管 Qt 的 qDebug/qWarning 等输出

详见：[日志系统使用说明](docs/LOGGER_USAGE.md)

---

## 🚀 构建和运行

### 环境要求

- **Qt**：6.9.3 或更高版本
- **CMake**：3.16 或更高版本
- **编译器**：MinGW 64-bit (Windows) 或 GCC/Clang (Linux/macOS)

### 构建步骤

#### 1. 配置项目

```bash
# 在项目根目录执行
cmake -S . -B build -G "Ninja"
```

如果使用 MinGW Makefiles：

```bash
cmake -S . -B build -G "MinGW Makefiles"
```

#### 2. 编译项目

```bash
# Debug 版本
cmake --build build --config Debug

# Release 版本
cmake --build build --config Release
```

#### 3. 运行程序

```bash
# Windows
.\bin\Debug\cursor_qt.exe

# Linux/macOS
./bin/Debug/cursor_qt
```

### 输出目录说明

- **bin/Debug/**：Debug 版本的可执行文件和依赖
- **bin/Release/**：Release 版本的可执行文件和依赖
- **build/**：CMake 生成的临时文件（可删除重新生成）

---

## ⚙️ 配置文件

### 位置

```
bin/Debug/config/config.json    # Debug 版本
bin/Release/config/config.json  # Release 版本
```

### 格式

```json
{
    "filePath1": "C:/path/to/file1.bin",
    "filePath2": "C:/path/to/file2.bin",
    "filePath3": "C:/path/to/file3.bin"
}
```


### 配置项说明

| 配置项 | 类型 | 说明 | 默认值 |
|--------|------|------|--------|
| `filePath1` | String | 第一个文件路径 | `""` |
| `filePath2` | String | 第二个文件路径 | `""` |
| `filePath3` | String | 第三个文件路径 | `""` |

### 注意事项

- 配置文件会在程序首次运行时自动创建
- 程序退出时会自动保存当前配置
- 如果配置文件损坏，删除后重新运行程序即可重新生成

---

## 📝 日志系统

### 日志文件位置

```
bin/Debug/logs/app.log    # Debug 版本
bin/Release/logs/app.log  # Release 版本
```

### 使用示例

```cpp
#include "logger.h"

// 在 main.cpp 中初始化
Logger::init(logPath, LogLevel::DEBUG, true, true);

// 在代码中使用
LOG_DEBUG() << "调试信息";
LOG_INFO() << "程序启动";
LOG_WARNING() << "警告信息";
LOG_ERROR() << "错误:" << errorCode;
LOG_FATAL() << "致命错误";  // 会终止程序
```

### 日志格式

```
2025-11-25 14:32:15.123 [INFO]     程序启动
2025-11-25 14:32:15.125 [DEBUG]    应用程序路径: D:/workdir/study/cursor_qt/bin/Debug
2025-11-25 14:32:16.456 [ERROR]    打开配置文件失败
```

### 重要提示

**所有日志宏都要用 `<<` 操作符，不能直接传参数！**

```cpp
// ❌ 错误写法
LOG_ERROR("打开配置文件失败");

// ✅ 正确写法
LOG_ERROR() << "打开配置文件失败";
```

详见：[日志系统使用说明](docs/LOGGER_USAGE.md)

---

## 📚 相关文档

- **[LOGGER_USAGE.md](docs/LOGGER_USAGE.md)** - 日志系统详细使用说明
  - 日志系统设计
  - 初始化和配置
  - 使用示例
  - 线程安全说明
  - 常见问题

- **[JSON_USAGE.md](docs/JSON_USAGE.md)** - Qt JSON 处理使用说明
  - QJsonDocument 详解
  - QJsonValue 使用方法
  - 读取和写入 JSON 文件
  - 最佳实践
  - 常见问题

---

## 🔧 开发说明

### 添加新功能

1. **添加 UI 控件**：在 Qt Designer 中编辑 `mainwindow.ui`
2. **添加槽函数**：在 `mainwindow.h` 中声明，在 `mainwindow.cpp` 中实现
3. **连接信号和槽**：在 `MainWindow` 构造函数中使用 `connect()`
4. **更新配置**：在 `loadConfig()` 和 `saveConfig()` 中添加新的配置项

### 代码规范

- 使用 Qt 命名规范（驼峰命名）
- UI 控件命名：`类型_功能`（如 `btn_get_file_path`）
- 私有成员变量：`m_` 前缀（如 `m_logFile`）
- 使用 Qt 的信号槽机制而非回调函数
- 使用 Qt 的容器类（QVector、QString 等）

### 调试技巧

```cpp
// 使用日志系统调试
LOG_DEBUG() << "变量值:" << value;

// 使用 Qt 的调试宏
qDebug() << "调试信息";  // 会被日志系统接管
```

### 配置文件读写示例

```cpp
// 在 mainwindow.cpp 中实现
void MainWindow::loadConfig()
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config/config.json";
    QFile file(configPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG_WARNING() << "配置文件不存在，使用默认配置";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull()) {
        LOG_ERROR() << "配置文件格式错误";
        return;
    }

    QJsonObject config = doc.object();
    ui->lineEdit_file_path->setText(config["filePath1"].toString());
    ui->lineEdit_file_path_2->setText(config["filePath2"].toString());
    ui->lineEdit_file_path_3->setText(config["filePath3"].toString());
}

void MainWindow::saveConfig()
{
    QJsonObject config;
    config["filePath1"] = ui->lineEdit_file_path->text();
    config["filePath2"] = ui->lineEdit_file_path_2->text();
    config["filePath3"] = ui->lineEdit_file_path_3->text();

    QJsonDocument doc(config);

    QString configPath = QCoreApplication::applicationDirPath() + "/config/config.json";
    QDir().mkpath(QFileInfo(configPath).absolutePath());

    QFile file(configPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        LOG_INFO() << "配置已保存";
    }
}
```

---

## 📄 许可证

本项目仅供学习和参考使用。

---

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

---

## 📧 联系方式

如有问题，请通过 GitHub Issues 联系。
