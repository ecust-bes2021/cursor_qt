# Qt 项目模板使用指南

本文档详细说明如何基于 `qt_tool_template` 模板创建新的 Qt 桌面应用程序项目。

---

## 📋 模板特性

本模板提供以下开箱即用的功能：

- ✅ **完善的 CMake 构建系统**：支持 Debug/Release 分离、自动部署 Qt 依赖
- ✅ **日志系统**：类似 Python logging 的日志功能，支持文件和控制台双输出
- ✅ **配置管理**：基于 JSON 的配置文件加载和保存
- ✅ **绿色软件设计**：所有配置和数据文件位于程序目录
- ✅ **跨平台支持**：基于 Qt 6，支持 Windows、Linux、macOS
- ✅ **详细文档**：包含日志系统和 JSON 处理的完整使用说明
- ✅ **规范的目录结构**：源码、头文件、UI、资源文件分类清晰

---

## 📁 项目目录结构

```
qt_tool_template/
├── src/                    # 源代码文件 (.cpp)
│   ├── main.cpp
│   ├── mainwindow.cpp
│   └── logger.cpp
├── include/                # 头文件 (.h)
│   ├── mainwindow.h
│   └── logger.h
├── ui/                     # UI 文件 (.ui)
│   └── mainwindow.ui
├── resources/              # 资源文件
│   ├── styles/             # 样式表
│   │   └── style.qss
│   └── icons/              # 图标（可选）
├── config/                 # 配置文件模板
│   └── config.json
├── docs/                   # 文档
│   ├── TEMPLATE_USAGE.md
│   ├── LOGGER_USAGE.md
│   └── JSON_USAGE.md
├── CMakeLists.txt          # CMake 构建配置
├── build/                  # 构建目录（自动生成，不提交到 Git）
└── bin/                    # 输出目录（自动生成，不提交到 Git）
    ├── Debug/
    │   ├── qt_tool_template.exe
    │   ├── config/         # 运行时配置
    │   ├── resources/      # 运行时资源
    │   ├── data/           # 数据文件（程序生成）
    │   └── logs/           # 日志文件（程序生成）
    └── Release/
```

---

## 🚀 快速开始：创建新项目

### 方法 1：直接复制（推荐）

#### 步骤 1：复制项目目录

```bash
# 复制整个项目到新目录
cp -r qt_tool_template my_new_project

# 或者在 Windows PowerShell 中
Copy-Item -Recurse qt_tool_template my_new_project

# 进入新项目目录
cd my_new_project
```

#### 步骤 2：修改项目名称

**只需修改一个文件中的一个变量！**

打开 `CMakeLists.txt`，找到第 6 行：

```cmake
# ========================================
# 项目名称配置（创建新项目时只需修改这里）
# ========================================
set(PROJECT_NAME "qt_tool_template")
```

将 `"qt_tool_template"` 改为你的项目名称，例如：

```cmake
set(PROJECT_NAME "my_new_project")
```

**就这么简单！** 所有其他地方都会自动使用这个变量。

#### 步骤 3：清理旧的构建文件

```bash
# 删除旧的构建目录和输出目录
rm -rf build bin

# Windows PowerShell
Remove-Item -Recurse -Force build, bin
```

#### 步骤 4：配置和构建项目

```bash
# 配置项目
cmake -S . -B build -G "Ninja"

# 编译项目（Debug 版本）
cmake --build build --config Debug

# 或编译 Release 版本
cmake --build build --config Release
```

> ⚠️ **重要提示（VS Code 用户）**：
>
> 如果你在 VS Code 中修改了 `PROJECT_NAME` 后出现 `ninja: error: unknown target 'qt_tool_template'` 错误，这是因为 CMake Tools 还在使用旧的 target 名称。
>
> **解决方法**：
> 1. 查看 VS Code 状态栏（底部），找到 CMake Tools 显示的 "当前 target"
> 2. 点击它，从下拉菜单中选择你的新项目名称（例如 `my_new_project`）
> 3. 或者直接在终端运行上面的命令，不使用 IDE 的构建按钮
>
> 这个问题只在改名后第一次构建时出现，之后就不会再有了。

#### 步骤 5：运行你的新项目

```bash
# Windows
.\bin\Debug\my_new_project.exe

# Linux/macOS
./bin/Debug/my_new_project
```

---

## 📝 自定义项目内容

### 1. 修改窗口标题和界面

#### 方法 A：使用 Qt Designer（推荐）

1. 用 Qt Designer 打开 `ui/mainwindow.ui`
2. 修改窗口标题、添加/删除控件
3. 保存后重新编译即可

#### 方法 B：在代码中修改

在 `src/mainwindow.cpp` 的构造函数中添加：

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("我的新工具");

    LOG_INFO() << "初始化主窗口";
    // ... 其他代码
}
```

### 2. 修改配置文件结构

#### 步骤 1：修改 `config.json` 模板

编辑 `config/config.json`：

```json
{
    "appName": "MyApp",
    "theme": "dark",
    "autoSave": true,
    "maxItems": 100
}
```

#### 步骤 2：修改 `loadConfig()` 和 `saveConfig()` 函数

在 `src/mainwindow.cpp` 中修改配置加载和保存逻辑。

详细的 JSON 处理方法请参考：[JSON_USAGE.md](JSON_USAGE.md)

### 3. 添加新的 UI 控件和功能

#### 步骤 1：在 Qt Designer 中添加控件

1. 打开 `ui/mainwindow.ui`
2. 从左侧工具箱拖拽控件到窗口
3. 设置控件的 `objectName`（例如：`btn_submit`、`lineEdit_username`）
4. 保存文件

#### 步骤 2：在 `include/mainwindow.h` 中声明槽函数

```cpp
private slots:
    void onBtnSubmitClicked();  // 新增的槽函数
```

#### 步骤 3：在 `src/mainwindow.cpp` 中实现槽函数

```cpp
// 在构造函数中连接信号和槽
connect(ui->btn_submit, &QPushButton::clicked, this, &MainWindow::onBtnSubmitClicked);

// 实现槽函数
void MainWindow::onBtnSubmitClicked()
{
    LOG_INFO() << "提交按钮被点击";
    // 处理你的业务逻辑
}
```

### 4. 使用日志系统

日志系统已经在 `src/main.cpp` 中初始化，你可以在任何地方使用：

#### 在任意 `.cpp` 文件中使用日志

```cpp
#include "logger.h"

void someFunction()
{
    LOG_DEBUG() << "调试信息，变量值=" << value;
    LOG_INFO() << "普通业务流程";
    LOG_WARNING() << "配置缺失，使用默认值";
    LOG_ERROR() << "发生错误，错误码=" << errorCode;
}
```

#### 日志级别说明

- **DEBUG**：调试信息，开发阶段使用
- **INFO**：一般信息，记录程序运行流程
- **WARNING**：警告信息，不影响程序运行但需要注意
- **ERROR**：错误信息，程序出现错误但可以继续运行
- **FATAL**：致命错误，程序无法继续运行（会终止程序）

#### 修改日志级别

在 `src/main.cpp` 中修改：

```cpp
// 只记录 INFO 及以上级别的日志（忽略 DEBUG）
Logger::init(logPath, LogLevel::INFO, true, true);

// 记录所有级别的日志（包括 DEBUG）
Logger::init(logPath, LogLevel::DEBUG, true, true);
```

详细使用说明请参考：[LOGGER_USAGE.md](LOGGER_USAGE.md)

### 5. 处理 JSON 数据

模板已经包含了完整的 JSON 处理示例，详细说明请参考：[JSON_USAGE.md](JSON_USAGE.md)

#### 快速示例：读取 JSON 文件

```cpp
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

void readJsonFile()
{
    QFile file("data/settings.json");
    if (!file.open(QIODevice::ReadOnly)) {
        LOG_ERROR() << "无法打开文件";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull()) {
        LOG_ERROR() << "JSON 格式错误";
        return;
    }

    QJsonObject obj = doc.object();
    QString value = obj["key"].toString();
    int number = obj["number"].toInt();

    LOG_INFO() << "读取成功:" << value << number;
}
```

#### 快速示例：写入 JSON 文件

```cpp
void writeJsonFile()
{
    QJsonObject obj;
    obj["key"] = "value";
    obj["number"] = 123;

    QJsonDocument doc(obj);

    QFile file("data/output.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        LOG_INFO() << "写入成功";
    }
}
```

---

## 🔧 高级定制

### 1. 添加新的源文件

#### 步骤 1：创建新文件

例如创建一个工具类：
- 头文件：`include/utils.h`
- 源文件：`src/utils.cpp`

#### 步骤 2：在 `CMakeLists.txt` 中添加

找到 `set(PROJECT_SOURCES ...)` 部分，添加你的文件：

```cmake
set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/logger.cpp
        src/utils.cpp           # 新增源文件
        include/mainwindow.h
        include/logger.h
        include/utils.h         # 新增头文件（Qt MOC 需要）
        ui/mainwindow.ui
)
```

> ⚠️ **重要**：头文件必须添加到 `PROJECT_SOURCES` 中，否则 Qt 的 MOC（元对象编译器）无法处理 `Q_OBJECT` 宏，会导致链接错误。

#### 步骤 3：重新配置和编译

```bash
cmake -S . -B build -G "Ninja"
cmake --build build --config Debug
```

### 2. 修改输出目录结构

默认情况下，程序编译后会自动：
1. 创建以下目录：`config/`、`data/`、`logs/`
2. 复制 `resources/` 文件夹（包含样式表、图标等）
3. 复制 `config/` 文件夹（包含配置文件）

如果需要添加新的运行时目录，在 `CMakeLists.txt` 中修改：

```cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/logs
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/output  # 新增目录
    COMMENT "Creating application directory structure..."
)
```

### 3. 添加新的资源文件

只需将文件放入 `resources/` 目录的相应子目录中，编译时会自动复制到输出目录：

- 样式表：放入 `resources/styles/`
- 图标：放入 `resources/icons/`
- 其他资源：在 `resources/` 下创建新目录

无需修改 `CMakeLists.txt`，因为整个 `resources/` 文件夹会被复制。

### 4. 修改 Qt 依赖部署选项

在 `CMakeLists.txt` 中找到 `windeployqt` 部分，可以调整部署选项：

```cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE}
        --no-translations          # 不复制翻译文件
        --no-system-d3d-compiler   # 不复制 D3D 编译器
        --no-opengl-sw             # 不复制 OpenGL 软件渲染
        --no-svg                   # 不复制 SVG 支持
        --no-network               # 不复制网络插件（如果需要网络功能，删除此行）
        --no-quick-import          # 不复制 QML 插件
        $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running windeployqt to copy Qt dependencies (minimal)..."
)
```


---

## ❓ 常见问题（FAQ）

### 1. 日志系统如何工作？

**问题**：在 `src/main.cpp` 中初始化日志系统后，为什么在 `src/mainwindow.cpp` 中可以直接使用日志宏？

**答案**：日志系统使用**单例模式**设计。`Logger::init()` 在 `src/main.cpp` 中初始化后，日志系统的单例实例就已经创建并配置好了。在任何其他文件中，只需要 `#include "logger.h"`，就可以使用 `LOG_INFO()` 等宏，这些宏会自动调用单例实例。

### 2. 如何修改项目名称？

**问题**：创建新项目时，需要修改哪些地方？

**答案**：**只需修改一个地方！** 打开 `CMakeLists.txt`，修改第 6 行的 `PROJECT_NAME` 变量即可：

```cmake
set(PROJECT_NAME "your_new_project_name")
```

所有其他地方（可执行文件名、目标名称等）都会自动使用这个变量。

**修改后的注意事项**：
- 删除 `build` 和 `bin` 目录，重新配置项目
- 如果在 VS Code 中使用，需要在状态栏切换 CMake target 到新的项目名称
- 或者直接在终端运行 `cmake --build build --config Debug` 命令

### 3. 如何添加第三方库？

**问题**：如何在项目中使用第三方库（如 OpenCV、Boost 等）？

**答案**：在 `CMakeLists.txt` 中添加 `find_package` 和 `target_link_libraries`：

```cmake
# 查找第三方库
find_package(OpenCV REQUIRED)

# 链接到你的项目
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS}  # 添加第三方库
)
```

### 4. 如何禁用控制台窗口？

**问题**：Release 版本如何隐藏控制台窗口？

**答案**：在 `CMakeLists.txt` 中修改 `WIN32_EXECUTABLE` 选项：

```cmake
set_target_properties(${PROJECT_NAME} PROPERTIES
    # ... 其他属性
    WIN32_EXECUTABLE TRUE  # 改为 TRUE 隐藏控制台
)
```

注意：隐藏控制台后，日志只会写入文件，不会显示在控制台。

### 5. 如何处理中文路径和中文字符？

**问题**：程序在处理中文路径时出现乱码怎么办？

**答案**：Qt 6 默认使用 UTF-8 编码，通常不会有问题。如果遇到问题，确保：

1. 源代码文件保存为 UTF-8 编码
2. 使用 `QString` 而不是 `std::string` 处理文本
3. 读写文件时使用 `QTextCodec`（如果需要）

### 6. 如何打包发布程序？

**问题**：如何将程序打包给其他用户使用？

**答案**：

1. 编译 Release 版本：
   ```bash
   cmake --build build --config Release
   ```

2. 程序和所有依赖都在 `bin/Release/` 目录下

3. 直接将整个 `bin/Release/` 目录打包成 zip 即可

4. 用户解压后直接运行 `.exe` 文件，无需安装

### 7. 如何调试程序？

**问题**：如何在 IDE 中调试程序？

**答案**：

1. 编译 Debug 版本
2. 在 IDE 中设置可执行文件路径为 `bin/Debug/your_project.exe`
3. 设置工作目录为 `bin/Debug/`
4. 设置断点并开始调试

或者直接在 Qt Creator 中打开 `CMakeLists.txt`，它会自动配置好调试环境。

### 8. VS Code 中修改项目名称后出现 "unknown target" 错误？

**问题**：修改 `PROJECT_NAME` 后，VS Code 构建时报错 `ninja: error: unknown target 'qt_tool_template'`

**答案**：这是 VS Code 的 CMake Tools 插件缓存了旧的 target 名称导致的。

**解决步骤**：

1. **查看状态栏**：在 VS Code 底部状态栏找到 CMake Tools 显示的 target 名称（通常显示为一个按钮）

2. **切换 target**：
   - 点击状态栏中的 target 名称按钮
   - 从下拉菜单中选择你的新项目名称（例如 `my_new_project`）
   - 或者选择 `[all]` 来构建所有目标

3. **重新构建**：点击 VS Code 的构建按钮或按 `Ctrl+Shift+B`

**或者使用终端**（推荐）：

```bash
# 直接在终端运行，不指定 target，会自动构建默认目标
cmake --build build --config Debug
```

**为什么会这样**：
- CMake 配置时会生成新的 target 名称
- 但 VS Code 的 CMake Tools 可能还在缓存旧的 target 列表
- 手动切换 target 或重启 VS Code 可以解决这个问题

---

## 📚 相关文档

- **[README.md](../README.md)** - 项目概述和功能说明
- **[LOGGER_USAGE.md](LOGGER_USAGE.md)** - 日志系统详细使用说明
- **[JSON_USAGE.md](JSON_USAGE.md)** - Qt JSON 处理详细教程

---

## 🎯 最佳实践建议

### 1. 代码组织

- 将业务逻辑和 UI 逻辑分离
- 创建独立的工具类处理通用功能
- 使用命名空间避免命名冲突

### 2. 错误处理

- 所有文件操作都要检查是否成功
- 使用日志记录错误信息
- 给用户友好的错误提示

### 3. 配置管理

- 配置文件使用 JSON 格式，便于阅读和修改
- 为所有配置项提供默认值
- 配置文件损坏时使用默认配置，不要让程序崩溃

### 4. 日志使用

- 开发阶段使用 `DEBUG` 级别
- 发布版本使用 `INFO` 级别
- 关键操作（文件读写、网络请求等）都要记录日志
- 不要在循环中大量输出日志

### 5. 性能优化

- 避免在主线程中执行耗时操作
- 使用 `QThread` 或 `QtConcurrent` 处理后台任务
- 大文件读写使用流式处理，不要一次性加载到内存

---

## 🚀 下一步

现在你已经了解了如何使用这个模板创建新项目！

建议的学习路径：

1. ✅ 按照本文档创建一个测试项目
2. ✅ 修改 UI 界面，添加自己的控件
3. ✅ 实现一个简单的功能（如文本处理、文件转换等）
4. ✅ 阅读 [LOGGER_USAGE.md](LOGGER_USAGE.md) 深入了解日志系统
5. ✅ 阅读 [JSON_USAGE.md](JSON_USAGE.md) 深入了解 JSON 处理
6. ✅ 根据需要添加更多功能模块

---

## 📧 反馈和贡献

如果你在使用过程中遇到问题，或者有改进建议，欢迎：

- 提交 Issue
- 提交 Pull Request
- 分享你基于此模板创建的项目

---

**祝你开发愉快！** 🎉

