# Qt 工具开发模板 (qt_tool_template)

一个专业的 Qt 6 桌面应用程序开发模板，包含完整的项目结构、日志系统、配置管理等开箱即用的功能。

## ✨ 主要特性

- 🏗️ **规范的项目结构**：源码、头文件、UI、资源文件分类清晰
- 📝 **完整的日志系统**：类似 Python logging，支持文件和控制台双输出
- ⚙️ **JSON 配置管理**：轻松加载和保存应用配置
- 🎨 **样式表支持**：内置 QSS 样式表加载机制
- 📦 **自动部署**：CMake 自动处理 Qt 依赖部署（Windows）
- 🟢 **绿色软件设计**：所有数据文件位于程序目录，无需安装
- 🔧 **跨平台支持**：基于 Qt 6，支持 Windows、Linux、macOS

## 🚀 快速开始

### 前置要求

- Qt 6.x（推荐 6.9.3 或更高版本）
- CMake 3.16+
- C++17 编译器（GCC、Clang 或 MSVC）
- Ninja 构建工具（可选，推荐）

### 编译和运行

```bash
# 1. 配置项目
cmake -S . -B build -G "Ninja"

# 2. 编译（Debug 版本）
cmake --build build --config Debug

# 3. 运行程序
./bin/Debug/qt_tool_template.exe
```

### 创建新项目

详细步骤请参考 [docs/TEMPLATE_USAGE.md](docs/TEMPLATE_USAGE.md)

快速步骤：
1. 复制整个项目目录
2. 修改 `CMakeLists.txt` 中的 `PROJECT_NAME` 变量
3. 删除 `build` 和 `bin` 目录
4. 重新编译

## 📁 项目结构

```
qt_tool_template/
├── src/              # 源代码文件 (.cpp)
├── include/          # 头文件 (.h)
├── ui/               # UI 文件 (.ui)
├── resources/        # 资源文件（样式表、图标等）
├── config/           # 配置文件模板
├── docs/             # 文档
├── CMakeLists.txt    # CMake 构建配置
└── bin/              # 输出目录（自动生成）
```

## 📚 文档

- **[TEMPLATE_USAGE.md](docs/TEMPLATE_USAGE.md)** - 详细的模板使用指南
- **[LOGGER_USAGE.md](docs/LOGGER_USAGE.md)** - 日志系统使用说明
- **[JSON_USAGE.md](docs/JSON_USAGE.md)** - JSON 处理教程

## 🎯 核心功能

### 日志系统

```cpp
#include "logger.h"

LOG_DEBUG() << "调试信息";
LOG_INFO() << "普通信息";
LOG_WARNING() << "警告信息";
LOG_ERROR() << "错误信息";
```

### 配置管理

```cpp
// 加载配置
QJsonObject config = loadConfigFromFile("config/config.json");

// 保存配置
saveConfigToFile(config, "config/config.json");
```

### 样式表

样式表自动从 `resources/styles/style.qss` 加载，修改后重新编译即可生效。

## 🔧 常见任务

### 添加新的源文件

1. 在 `src/` 目录创建 `.cpp` 文件
2. 在 `include/` 目录创建 `.h` 文件
3. 在 `CMakeLists.txt` 的 `PROJECT_SOURCES` 中添加这两个文件
4. 重新编译

### 添加新的资源文件

1. 将文件放入 `resources/` 目录的相应子目录
2. 重新编译（无需修改 CMakeLists.txt）

### 修改项目名称

1. 打开 `CMakeLists.txt`
2. 修改第 6 行的 `PROJECT_NAME` 变量
3. 删除 `build` 和 `bin` 目录
4. 重新编译

## 📋 系统要求

| 组件 | 版本 | 说明 |
|------|------|------|
| Qt | 6.x | 核心框架 |
| CMake | 3.16+ | 构建系统 |
| C++ | C++17 | 编程语言 |
| 编译器 | GCC/Clang/MSVC | 任选其一 |

## 🐛 故障排除

### 编译错误：`undefined reference to 'MainWindow::staticMetaObject'`

**原因**：头文件未添加到 `PROJECT_SOURCES`

**解决**：在 `CMakeLists.txt` 中确保所有包含 `Q_OBJECT` 的头文件都在 `PROJECT_SOURCES` 中

### 找不到 UI 文件

**原因**：CMake 的 AUTOUIC 找不到 UI 文件位置

**解决**：确保 `CMAKE_AUTOUIC_SEARCH_PATHS` 包含 `ui/` 目录

## 💡 最佳实践

- 将业务逻辑和 UI 逻辑分离
- 使用日志记录关键操作
- 为所有配置项提供默认值
- 定期备份配置文件
- 使用 Qt Designer 设计 UI

## 📄 许可证

MIT License

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📞 联系方式

如有问题或建议，欢迎通过以下方式联系：
- 提交 GitHub Issue
- 发送邮件反馈

---

**祝你开发愉快！** 🎉

