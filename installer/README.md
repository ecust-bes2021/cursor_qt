# Inno Setup 安装程序使用文档

本项目使用 **Inno Setup** 创建 Windows 安装程序，并通过 CMake 模板化配置实现**单一信息源**管理。

---

## 📁 文件结构

```
installer/
├── setup.iss.in    # 模板文件（不要删除）
├── setup.iss       # 自动生成的安装脚本（由 CMake 生成，勿手动编辑）
├── output/         # 安装程序输出目录
│   └── xxx-Setup.exe
└── README.md       # 本文档
```

---

## ⚙️ 配置说明

所有配置都在 `CMakeLists.txt` 顶部，修改后运行 `cmake -B build` 即可自动同步到安装脚本。

### 可配置项

| 配置项 | 位置 | 说明 | 示例 |
|--------|------|------|------|
| `PROJECT_NAME` | 第6行 | 应用名称 | `"MyApp"` |
| `VERSION` | 第8行 | 版本号（三位） | `1.0.0` |
| `APP_PUBLISHER` | 第14行 | 发布者/公司名 | `"BES"` |
| `APP_DESCRIPTION` | 第15行 | 应用描述 | `"我的应用程序"` |
| `APP_URL` | 第16行 | 官网/支持网址 | `"https://example.com"` |
| `APP_GUID` | 第17行 | 唯一标识符 | `"{{GUID}}"` |

### 配置示例

```cmake
# CMakeLists.txt 顶部

set(PROJECT_NAME "MyAwesomeApp")
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# 安装程序配置
set(APP_PUBLISHER "BES")
set(APP_DESCRIPTION "一个很棒的桌面应用程序")
set(APP_URL "https://bes.com/myapp")
set(APP_GUID "{{3F2504E0-4F89-11D3-9A0C-0305E82C3301}")
```

---

## 🚀 使用方法

### 前置条件

1. **安装 Inno Setup**
   - 下载：https://jrsoftware.org/isinfo.php
   - 或使用命令：`winget install JRSoftware.InnoSetup`

2. **编译 Release 版本**（安装程序只打包 Release）

### 构建步骤

```powershell
# 1. 配置项目（自动生成 setup.iss）
cmake -B build

# 2. 编译 Release 版本
cmake --build build --config Release

# 3. 构建安装程序
cmake --build build --target installer
```

### 输出位置

```
installer/output/[应用名]-[版本号]-Setup.exe
# 例如：installer/output/MyAwesomeApp-1.0.0-Setup.exe
```

---

## 🔧 高级配置

### 生成新的 GUID

每个应用必须有唯一的 GUID，否则会与其他应用冲突：

```powershell
# PowerShell 生成
[guid]::NewGuid().ToString().ToUpper()
```

将输出的 GUID 填入（注意开头双花括号）：
```cmake
set(APP_GUID "{{你的GUID}}")
```

### 添加应用图标

1. 准备 `.ico` 图标文件，放到 `resources/icons/app.ico`
2. 编辑 `installer/setup.iss.in`，取消注释：

```iss
SetupIconFile={#SourceDir}\resources\icons\app.ico
UninstallDisplayIcon={app}\{#MyAppExeName}
```

### 静默安装

```cmd
# 完全静默
MyApp-1.0.0-Setup.exe /VERYSILENT /SUPPRESSMSGBOXES

# 静默但显示进度
MyApp-1.0.0-Setup.exe /SILENT

# 指定安装目录
MyApp-1.0.0-Setup.exe /DIR="D:\MyApps\MyApp"
```

---

## 📦 安装程序功能

| 功能 | 状态 |
|------|------|
| 现代化安装向导界面 | ✅ |
| 中文/英文语言选择 | ✅ |
| 自定义安装目录 | ✅ |
| 创建桌面快捷方式（可选） | ✅ |
| 创建开始菜单项 | ✅ |
| 检测旧版本安装 | ✅ |
| 卸载程序（控制面板） | ✅ |
| 安装后运行程序（可选） | ✅ |
| LZMA2 高压缩率 | ✅ |

---

## ❓ 常见问题

### Q: 找不到 Inno Setup？
确保已安装 Inno Setup 6，CMake 会自动从注册表读取安装路径。

### Q: 安装程序太大？
Release 编译时会自动调用 `windeployqt` 复制 Qt DLL，可在 CMakeLists.txt 中调整选项减少依赖。

### Q: 如何修改安装界面文字？
编辑 `installer/setup.iss.in` 中的 `[Tasks]`、`[Icons]` 等部分。

### Q: 更新版本号后 setup.iss 没变？
重新运行 `cmake -B build` 触发 configure_file 更新。

---

## 📋 新项目检查清单

- [ ] 修改 `PROJECT_NAME`
- [ ] 修改 `VERSION`
- [ ] 修改 `APP_PUBLISHER`
- [ ] 修改 `APP_DESCRIPTION`
- [ ] 修改 `APP_URL`
- [ ] **生成并修改 `APP_GUID`**（必须！）
- [ ] 添加应用图标（可选）
- [ ] 测试安装/卸载流程

