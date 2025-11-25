# Qt JSON 处理使用说明（QJsonDocument）

本文档详细说明 Qt 中 JSON 处理的核心类 `QJsonDocument` 的作用、使用方法和最佳实践。

---

## 1. QJsonDocument 的核心作用

`QJsonDocument` 是 Qt 中处理 JSON 的**核心转换器**，它在 **JSON 文本**（QString/QByteArray）和 **JSON 对象**（QJsonObject/QJsonArray）之间架起桥梁。

### 为什么需要 QJsonDocument？

假设你有一个 JSON 配置文件 `config.json`：

```json
{
    "appName": "MyApp",
    "version": "1.0.0",
    "maxUsers": 100
}
```

读取流程：

```
文件 (config.json)
    ↓ file.readAll()
QByteArray (纯文本: "{\"appName\":\"MyApp\",...}")
    ↓ QJsonDocument::fromJson()  ← QJsonDocument 在这里工作
QJsonDocument (已解析的 JSON 文档)
    ↓ .object()
QJsonObject (可以操作的对象)
    ↓ obj["appName"].toString()
QString ("MyApp")
```

**QJsonDocument 负责第 2 步：把文本解析成可操作的对象。**

---

## 2. 读取 JSON 文件的完整示例

### 示例 1：读取配置文件

```cpp
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

void readConfig()
{
    // 1. 从文件读取文本
    QFile file("config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件";
        return;
    }
    QByteArray jsonData = file.readAll();  // 读到的是纯文本
    file.close();

    // 2. 用 QJsonDocument 解析文本 → JSON 对象
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning() << "JSON 格式错误";
        return;
    }

    // 3. 从 QJsonDocument 中取出 QJsonObject
    QJsonObject obj = doc.object();

    // 4. 读取具体的值
    QString appName = obj["appName"].toString();
    QString version = obj["version"].toString();
    int maxUsers = obj["maxUsers"].toInt();

    qDebug() << "应用名称:" << appName;
    qDebug() << "版本:" << version;
    qDebug() << "最大用户数:" << maxUsers;
}
```

### 示例 2：读取包含数组的 JSON

假设 `users.json` 内容：

```json
{
    "users": [
        {"name": "Alice", "age": 25},
        {"name": "Bob", "age": 30}
    ]
}
```

读取代码：

```cpp
#include <QJsonArray>

void readUsers()
{
    QFile file("users.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    QJsonArray users = root["users"].toArray();

    for (const QJsonValue &value : users) {
        QJsonObject user = value.toObject();
        QString name = user["name"].toString();
        int age = user["age"].toInt();

        qDebug() << "姓名:" << name << ", 年龄:" << age;
    }

    file.close();
}
```

---

## 3. 写入 JSON 文件的完整示例

### 示例 1：保存配置文件

```cpp
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

void writeConfig()
{
    // 1. 构造 QJsonObject
    QJsonObject obj;
    obj["appName"] = "MyApp";
    obj["version"] = "1.0.0";
    obj["maxUsers"] = 100;

    // 2. 用 QJsonDocument 包装 QJsonObject
    QJsonDocument doc(obj);

    // 3. 转换成 JSON 文本（QByteArray）
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
    //                                ↑ 格式化输出（带缩进，便于阅读）

    // 4. 写入文件
    QFile file("config.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonData);
        file.close();
    }
}
```

生成的 `config.json`（格式化后）：

```json
{
    "appName": "MyApp",
    "version": "1.0.0",
    "maxUsers": 100
}
```

### 示例 2：保存包含数组的 JSON

```cpp
#include <QJsonArray>

void writeUsers()
{
    // 构造用户数组
    QJsonArray users;

    QJsonObject user1;
    user1["name"] = "Alice";
    user1["age"] = 25;
    users.append(user1);

    QJsonObject user2;
    user2["name"] = "Bob";
    user2["age"] = 30;
    users.append(user2);

    // 构造根对象
    QJsonObject root;
    root["users"] = users;

    // 转换并保存
    QJsonDocument doc(root);
    QFile file("users.json");
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}
```

---

## 4. QJsonDocument 的三大功能

### 功能 1：解析 JSON 文本（反序列化）

```cpp
QByteArray jsonText = "{\"name\":\"Alice\",\"age\":25}";
QJsonDocument doc = QJsonDocument::fromJson(jsonText);

if (!doc.isNull()) {
    QJsonObject obj = doc.object();
    qDebug() << obj["name"].toString();  // "Alice"
    qDebug() << obj["age"].toInt();      // 25
}
```

**关键方法：**
- `QJsonDocument::fromJson(QByteArray)` - 从 JSON 文本解析
- `.isNull()` - 检查解析是否成功
- `.object()` - 获取 QJsonObject
- `.array()` - 获取 QJsonArray（如果根是数组）

### 功能 2：生成 JSON 文本（序列化）

```cpp
QJsonObject obj;
obj["name"] = "Alice";
obj["age"] = 25;

QJsonDocument doc(obj);

// 紧凑格式（无空格）
QByteArray compact = doc.toJson();
// 输出: {"name":"Alice","age":25}

// 格式化输出（带缩进）
QByteArray formatted = doc.toJson(QJsonDocument::Indented);
// 输出:
// {
//     "name": "Alice",
//     "age": 25
// }
```

**关键方法：**
- `QJsonDocument(QJsonObject)` - 从对象构造文档
- `QJsonDocument(QJsonArray)` - 从数组构造文档
- `.toJson()` - 转换成紧凑 JSON 文本
- `.toJson(QJsonDocument::Indented)` - 转换成格式化 JSON 文本

### 功能 3：验证 JSON 格式

```cpp
QByteArray invalidJson = "{name: Alice}";  // 错误：键没有引号
QJsonDocument doc = QJsonDocument::fromJson(invalidJson);

if (doc.isNull()) {
    qDebug() << "JSON 格式错误！";
}
```

**常见 JSON 格式错误：**
- 键没有用双引号：`{name: "Alice"}` ❌ 应该是 `{"name": "Alice"}` ✅
- 使用单引号：`{'name': 'Alice'}` ❌ 应该用双引号 ✅
- 末尾多余逗号：`{"name": "Alice",}` ❌
- 注释：`{"name": "Alice" /* 注释 */}` ❌ JSON 不支持注释

---

## 5. 为什么不能直接用 QJsonObject？

你可能会想：为什么不能直接从文件读取到 QJsonObject？

**答案：QJsonObject 只能表示 JSON 对象，不能处理文本。**

```cpp
// ❌ 错误：QJsonObject 没有 fromJson() 方法
QJsonObject obj = QJsonObject::fromJson(jsonText);  // 编译错误！

// ✅ 正确：必须通过 QJsonDocument
QJsonDocument doc = QJsonDocument::fromJson(jsonText);
QJsonObject obj = doc.object();
```

### QJsonDocument 的设计理念

Qt 的 JSON 类采用了**职责分离**的设计：

- **QJsonObject** / **QJsonArray**：内存中的 JSON 数据结构（用于操作数据）
- **QJsonDocument**：文档容器（用于序列化/反序列化）
- **QJsonValue**：万能包装盒（统一存储不同类型的值）

类似的设计模式：
- `QDomDocument`（XML 文档）和 `QDomElement`（XML 元素）
- `QTextDocument`（富文本文档）和 `QTextBlock`（文本块）

---

## 6. QJsonValue 详解：为什么需要它？

### 6.1 核心问题：为什么不能只用 QJsonObject？

你可能会疑惑：既然可以直接用 `obj["key"] = value` 的方式添加数据，为什么还需要 QJsonValue？

**答案：QJsonValue 是"万能包装盒"，用于统一存储不同类型的值。**

### 6.2 QJsonValue 的作用

JSON 中的值可以是多种类型：
- 字符串：`"Alice"`
- 数字：`25`、`3.14`
- 布尔：`true`、`false`
- 对象：`{"key": "value"}`
- 数组：`[1, 2, 3]`
- null：`null`

**QJsonObject 需要一个统一的类型来存储这些不同类型的值**，这个统一的类型就是 `QJsonValue`。

### 6.3 类比：QJsonValue 就像"包装盒"

```
QJsonObject 是一个仓库
├── "name" → 📦 QJsonValue(包含 QString "Alice")
├── "age"  → 📦 QJsonValue(包含 int 25)
├── "tags" → 📦 QJsonValue(包含 QJsonArray [...])
└── "info" → 📦 QJsonValue(包含 QJsonObject {...})
```

- **写入时**：你放进去的任何值都会被自动装进"包装盒"（QJsonValue）
- **读取时**：你拿到的是"包装盒"（QJsonValue），需要打开盒子才能拿到真正的值

### 6.4 写入时：通常不需要显式使用 QJsonValue

```cpp
QJsonObject obj;

// ✅ 简洁写法（自动转换）
obj["name"] = "Alice";
obj["age"] = 25;
obj["active"] = true;

// ❌ 繁琐写法（显式使用 QJsonValue，没必要）
obj["name"] = QJsonValue("Alice");
obj["age"] = QJsonValue(25);
```

C++ 会自动把 `QString`、`int`、`bool` 等类型转换成 `QJsonValue`。

### 6.5 读取时：必须通过 QJsonValue

#### 场景 1：不知道值的类型

假设你从网络 API 收到一个 JSON：

```json
{
    "status": "success",
    "data": {
        "userId": 123,
        "username": "Alice"
    }
}
```

你不确定 `data` 字段是对象还是数组，这时需要用 QJsonValue 检查：

```cpp
QJsonObject response = doc.object();

// 取出 "data" 字段，得到的是 QJsonValue
QJsonValue dataValue = response["data"];

// 检查类型
if (dataValue.isObject()) {
    QJsonObject data = dataValue.toObject();
    int userId = data["userId"].toInt();
    qDebug() << "用户 ID:" << userId;
} else if (dataValue.isArray()) {
    QJsonArray data = dataValue.toArray();
    qDebug() << "数据是数组，长度:" << data.size();
} else if (dataValue.isNull()) {
    qDebug() << "数据为空";
}
```

**如果没有 QJsonValue，你无法安全地检查类型！**

#### 场景 2：遍历数组

```cpp
QJsonArray users = root["users"].toArray();

for (const QJsonValue &value : users) {  // ← 这里必须用 QJsonValue
    if (value.isObject()) {
        QJsonObject user = value.toObject();
        qDebug() << user["name"].toString();
    }
}
```

**数组中的每个元素都是 QJsonValue 类型**，因为数组可能包含不同类型的值：

```json
[
    "Alice",
    25,
    true,
    {"key": "value"},
    [1, 2, 3]
]
```

#### 场景 3：处理可能不存在的字段

```cpp
QJsonObject config = doc.object();

// 方法 1：使用 value() 返回 QJsonValue
QJsonValue themeValue = config.value("theme");
if (!themeValue.isUndefined()) {
    QString theme = themeValue.toString();
} else {
    qDebug() << "theme 字段不存在";
}

// 方法 2：直接用 contains() 检查
if (config.contains("theme")) {
    QString theme = config["theme"].toString();
}
```

### 6.6 QJsonValue 的核心方法

#### 类型检查方法

```cpp
QJsonValue value = obj["someKey"];

value.isNull()       // 是否为 null
value.isBool()       // 是否为布尔值
value.isDouble()     // 是否为数字（整数或浮点数）
value.isString()     // 是否为字符串
value.isArray()      // 是否为数组
value.isObject()     // 是否为对象
value.isUndefined()  // 是否未定义（键不存在）
```

#### 类型转换方法

```cpp
value.toBool()       // 转布尔值（如果不是布尔，返回 false）
value.toInt()        // 转整数（如果不是数字，返回 0）
value.toDouble()     // 转浮点数（如果不是数字，返回 0.0）
value.toString()     // 转字符串（如果不是字符串，返回空字符串）
value.toArray()      // 转数组（如果不是数组，返回空数组）
value.toObject()     // 转对象（如果不是对象，返回空对象）
```

#### 带默认值的转换

```cpp
QString name = obj["name"].toString("Unknown");  // 如果不存在或不是字符串，返回 "Unknown"
int age = obj["age"].toInt(18);                  // 如果不存在或不是数字，返回 18
bool active = obj["active"].toBool(false);       // 如果不存在或不是布尔，返回 false
```

### 6.7 安全读取 JSON 的最佳实践

#### ❌ 不安全的写法

```cpp
// 如果 JSON 格式不符合预期，可能得到错误的值
QString name = obj["name"].toString();  // 如果 "name" 是数字，返回空字符串
int age = obj["age"].toInt();           // 如果 "age" 是字符串 "25"，返回 0
```

#### ✅ 安全的写法

```cpp
// 检查类型后再转换
QJsonValue nameValue = obj["name"];
if (nameValue.isString()) {
    QString name = nameValue.toString();
    qDebug() << "姓名:" << name;
} else {
    qWarning() << "name 字段类型错误";
}

QJsonValue ageValue = obj["age"];
if (ageValue.isDouble()) {
    int age = ageValue.toInt();
    qDebug() << "年龄:" << age;
} else {
    qWarning() << "age 字段类型错误";
}
```

### 6.8 QJsonValue 使用总结

| 场景 | 是否需要显式使用 QJsonValue | 原因 |
|------|---------------------------|------|
| **写入数据** | ❌ 通常不需要 | C++ 会自动转换 |
| **读取数据** | ✅ 必须使用 | 需要检查类型、处理不同类型的值 |
| **遍历数组** | ✅ 必须使用 | 数组元素是 QJsonValue 类型 |
| **检查字段是否存在** | ✅ 建议使用 | 可以用 `isUndefined()` 判断 |
| **处理可能为 null 的值** | ✅ 必须使用 | 需要用 `isNull()` 检查 |

**QJsonValue 的设计目的：**
1. **类型安全**：可以在运行时检查值的类型
2. **统一接口**：QJsonObject 和 QJsonArray 只需要处理一种类型
3. **支持 null**：可以表示 JSON 中的 `null` 值
4. **支持未定义**：可以表示键不存在的情况

**记住：**
- 写入时，QJsonValue 在幕后工作（自动转换）
- 读取时，QJsonValue 在前台工作（你需要显式使用）

---

## 7. 实际使用场景

### 场景 1：读取应用配置文件

```cpp
// config/settings.json
// {
//     "theme": "dark",
//     "fontSize": 14,
//     "autoSave": true
// }

void loadSettings()
{
    QFile file("config/settings.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "配置文件不存在，使用默认配置";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull()) {
        qWarning() << "配置文件格式错误";
        return;
    }

    QJsonObject config = doc.object();
    QString theme = config["theme"].toString("light");  // 默认值 "light"
    int fontSize = config["fontSize"].toInt(12);        // 默认值 12
    bool autoSave = config["autoSave"].toBool(false);   // 默认值 false

    // 应用配置
    applyTheme(theme);
    setFontSize(fontSize);
    enableAutoSave(autoSave);
}
```

### 场景 2：保存用户数据

```cpp
void saveUserProfile(const QString &username, const QString &email, int loginCount)
{
    QJsonObject user;
    user["username"] = username;
    user["email"] = email;
    user["loginCount"] = loginCount;
    user["lastLogin"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument doc(user);

    QFile file("data/user_profile.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}
```

### 场景 3：网络 API 通信

```cpp
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

// 发送 JSON 请求
void sendLoginRequest(const QString &username, const QString &password)
{
    // 构造 JSON 请求体
    QJsonObject request;
    request["action"] = "login";
    request["username"] = username;
    request["password"] = password;

    QJsonDocument doc(request);
    QByteArray jsonData = doc.toJson();

    // 发送 HTTP POST 请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("https://api.example.com/auth"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->post(req, jsonData);

    // 处理响应
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            QJsonObject responseObj = responseDoc.object();

            bool success = responseObj["success"].toBool();
            QString message = responseObj["message"].toString();

            if (success) {
                qDebug() << "登录成功:" << message;
            } else {
                qWarning() << "登录失败:" << message;
            }
        }
        reply->deleteLater();
    });
}
```

### 场景 4：处理嵌套 JSON 结构

```cpp
// 复杂的 JSON 结构
// {
//     "company": "TechCorp",
//     "employees": [
//         {
//             "name": "Alice",
//             "department": "Engineering",
//             "skills": ["C++", "Qt", "Python"]
//         },
//         {
//             "name": "Bob",
//             "department": "Design",
//             "skills": ["Photoshop", "Figma"]
//         }
//     ]
// }

void parseCompanyData()
{
    QFile file("company.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    QString company = root["company"].toString();
    QJsonArray employees = root["employees"].toArray();

    qDebug() << "公司:" << company;

    for (const QJsonValue &empValue : employees) {
        QJsonObject emp = empValue.toObject();
        QString name = emp["name"].toString();
        QString dept = emp["department"].toString();
        QJsonArray skills = emp["skills"].toArray();

        qDebug() << "员工:" << name << "部门:" << dept;
        qDebug() << "技能:";

        for (const QJsonValue &skillValue : skills) {
            qDebug() << "  -" << skillValue.toString();
        }
    }

    file.close();
}
```

---

## 8. Qt JSON 类之间的关系

| 类 | 作用 | 典型用法 | 说明 |
|---|------|----------|------|
| **QJsonDocument** | JSON 文本 ↔ JSON 对象的转换器 | `fromJson()` / `toJson()` | 负责序列化和反序列化 |
| **QJsonObject** | 表示 JSON 对象 `{...}` | `obj["key"] = value` | 键值对集合 |
| **QJsonArray** | 表示 JSON 数组 `[...]` | `arr.append(value)` | 有序值列表 |
| **QJsonValue** | 万能包装盒，统一存储不同类型的值 | `value.toString()` | 可以是字符串/数字/布尔/对象/数组/null |

### 类之间的协作关系

```
QJsonDocument (文档容器)
    ├── 包含 QJsonObject (根对象)
    │       ├── "name" → QJsonValue(QString)
    │       ├── "age"  → QJsonValue(int)
    │       └── "tags" → QJsonValue(QJsonArray)
    │                         ├── QJsonValue(QString)
    │                         ├── QJsonValue(QString)
    │                         └── QJsonValue(QString)
    │
    └── 或包含 QJsonArray (根数组)
            ├── QJsonValue(QJsonObject)
            ├── QJsonValue(QString)
            └── QJsonValue(int)
```

### 类型转换关系

```cpp
// QJsonValue → 具体类型
QJsonValue value = obj["key"];
QString str = value.toString();           // 转字符串
int num = value.toInt();                  // 转整数
double dbl = value.toDouble();            // 转浮点数
bool flag = value.toBool();               // 转布尔值
QJsonObject subObj = value.toObject();    // 转对象
QJsonArray subArr = value.toArray();      // 转数组

// 具体类型 → QJsonValue（自动转换）
obj["name"] = "Alice";                    // QString → QJsonValue
obj["age"] = 25;                          // int → QJsonValue
obj["score"] = 98.5;                      // double → QJsonValue
obj["active"] = true;                     // bool → QJsonValue
```

---

## 9. 常见问题（FAQ）

### 8.1 如何判断 JSON 中某个键是否存在？

```cpp
QJsonObject obj = doc.object();

if (obj.contains("username")) {
    QString username = obj["username"].toString();
} else {
    qWarning() << "缺少 username 字段";
}
```

### 8.2 如何处理可选字段（提供默认值）？

```cpp
// 方法 1：使用 value() 方法
QString theme = obj.value("theme").toString("light");  // 默认 "light"
int fontSize = obj.value("fontSize").toInt(12);        // 默认 12

// 方法 2：先检查再读取
QString theme = obj.contains("theme")
    ? obj["theme"].toString()
    : "light";
```

### 8.3 如何修改已有的 JSON 对象？

```cpp
QJsonObject obj = doc.object();

// 修改现有字段
obj["version"] = "2.0.0";

// 添加新字段
obj["newFeature"] = true;

// 删除字段
obj.remove("oldField");

// 保存修改
QJsonDocument newDoc(obj);
file.write(newDoc.toJson());
```

### 8.4 如何处理 JSON 解析错误？

```cpp
QJsonParseError error;
QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

if (error.error != QJsonParseError::NoError) {
    qWarning() << "JSON 解析错误:";
    qWarning() << "  位置:" << error.offset;
    qWarning() << "  错误:" << error.errorString();
    return;
}
```

### 9.5 QJsonDocument 和 QVariant 的关系？

```cpp
// QJsonObject 可以转换为 QVariantMap
QJsonObject obj = doc.object();
QVariantMap map = obj.toVariantMap();

// QVariantMap 可以转换为 QJsonObject
QVariantMap map;
map["name"] = "Alice";
map["age"] = 25;
QJsonObject obj = QJsonObject::fromVariantMap(map);
```

### 9.6 为什么需要 QJsonValue？（常见疑问）

**问题**：既然可以直接用 `obj["key"] = value` 添加数据，为什么还需要 QJsonValue？

**答案**：
- **写入时**：通常不需要显式使用 QJsonValue（自动转换）
- **读取时**：必须通过 QJsonValue 来检查类型和安全转换
- **设计目的**：QJsonValue 是"万能包装盒"，统一存储不同类型的值

详见第 6 节《QJsonValue 详解》。

---

## 10. 最佳实践

### ✅ 推荐做法

1. **总是检查文件是否成功打开**
   ```cpp
   if (!file.open(QIODevice::ReadOnly)) {
       qWarning() << "文件打开失败";
       return;
   }
   ```

2. **总是验证 JSON 解析是否成功**
   ```cpp
   if (doc.isNull()) {
       qWarning() << "JSON 格式错误";
       return;
   }
   ```

3. **使用格式化输出便于调试**
   ```cpp
   file.write(doc.toJson(QJsonDocument::Indented));  // 便于人类阅读
   ```

4. **为可选字段提供默认值**
   ```cpp
   int timeout = obj.value("timeout").toInt(30);  // 默认 30 秒
   ```

### ❌ 避免的做法

1. **不检查文件/解析错误**
   ```cpp
   // ❌ 危险：如果文件不存在会崩溃
   QFile file("config.json");
   file.open(QIODevice::ReadOnly);
   QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
   QJsonObject obj = doc.object();  // doc 可能是 null
   ```

2. **不验证字段类型**
   ```cpp
   // ❌ 危险：如果 "age" 是字符串会返回 0
   int age = obj["age"].toInt();

   // ✅ 更好：先检查类型
   if (obj["age"].isDouble()) {
       int age = obj["age"].toInt();
   }
   ```

3. **硬编码文件路径**
   ```cpp
   // ❌ 不推荐：路径可能在不同系统上不同
   QFile file("C:/Users/Alice/config.json");

   // ✅ 推荐：使用相对路径或应用程序目录
   QString path = QCoreApplication::applicationDirPath() + "/config/settings.json";
   QFile file(path);
   ```

---

## 11. 总结

### 记住这个流程

```
读取：文件 → QByteArray → QJsonDocument → QJsonObject → 数据
写入：数据 → QJsonObject → QJsonDocument → QByteArray → 文件
```

### 核心要点

- **QJsonDocument** 是文本和对象之间的**转换器**
- 使用 `fromJson()` 解析 JSON 文本
- 使用 `toJson()` 生成 JSON 文本
- 总是检查 `isNull()` 验证解析是否成功
- 使用 `QJsonDocument::Indented` 生成易读的格式化 JSON

### 相关文档

- Qt 官方文档：[QJsonDocument](https://doc.qt.io/qt-6/qjsondocument.html)
- Qt 官方文档：[QJsonObject](https://doc.qt.io/qt-6/qjsonobject.html)
- Qt 官方文档：[QJsonArray](https://doc.qt.io/qt-6/qjsonarray.html)
- 项目日志系统文档：`LOGGER_USAGE.md`

---

如果需要更高级的 JSON 处理功能（如 JSON Schema 验证、JSON Patch 等），可以考虑使用第三方库如 `nlohmann/json` 或 `RapidJSON`。但对于大多数 Qt 应用来说，Qt 内置的 JSON 类已经足够强大和易用。


