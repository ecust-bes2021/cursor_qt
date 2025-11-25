#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LOG_INFO() << "初始化主窗口";

    //加载配置文件
    loadConfig();

    //连接信号和槽
    connect(ui->btn_get_file_path, &QPushButton::clicked, this, &MainWindow::onBtnGetFilePathClicked);
    connect(ui->btn_get_file_path_2, &QPushButton::clicked, this, &MainWindow::onBtnGetFilePathClicked);
    connect(ui->btn_get_file_path_3, &QPushButton::clicked, this, &MainWindow::onBtnGetFilePathClicked);

    LOG_INFO() << "主窗口初始化完成";
}

MainWindow::~MainWindow()
{
    saveConfig();
    delete ui;
}

void MainWindow::onBtnGetFilePathClicked()
{
    // QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "All Files (*.*)");
    //读取bin文件内容
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "Bin Files (*.*)");
    if (filePath.isEmpty())
        return;
    // 判断是哪个按钮触发，写入对应的lineedit
    QObject *senderObj = sender();
    if (senderObj == ui->btn_get_file_path)
        ui->lineEdit_file_path->setText(filePath);
    else if (senderObj == ui->btn_get_file_path_2)
        ui->lineEdit_file_path_2->setText(filePath);
    else if (senderObj == ui->btn_get_file_path_3)
        ui->lineEdit_file_path_3->setText(filePath);
}

void MainWindow::loadConfig()
{
    //1. 构建配置文件路径
    QString configPath = QCoreApplication::applicationDirPath() + "/config/config.json";
    //2. 打开配置文件
    QFile file(configPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_ERROR() << "打开配置文件失败";
        return;
    }
    //3. 读取JSON数据
    QByteArray jsonData = file.readAll();
    file.close();

    //4. 解析JSON数据
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if(doc.isNull())
    {
        LOG_ERROR() << "解析配置文件失败";
        return;
    }

    //5. 获取json对象
    QJsonObject configObj = doc.object();

    //6. 读取配置并设置到UI控件
    if(configObj.contains("filePath1")){
        ui->lineEdit_file_path->setText(configObj["filePath1"].toString());
    }

    if(configObj.contains("filePath2")){
        ui->lineEdit_file_path_2->setText(configObj["filePath2"].toString());
    }
    if(configObj.contains("filePath3")){
        ui->lineEdit_file_path_3->setText(configObj["filePath3"].toString());
    }

    LOG_INFO() << "加载配置文件成功";

}

void MainWindow::saveConfig(){
    //1. 构建JSon对象
    QJsonObject configObj;
    configObj["filePath1"] = ui->lineEdit_file_path->text();
    configObj["filePath2"] = ui->lineEdit_file_path_2->text();
    configObj["filePath3"] = ui->lineEdit_file_path_3->text();

    //2. 构建JSON文档
    QJsonDocument doc(configObj);

    //3. 构建配置文件路径
    QString configPath = QCoreApplication::applicationDirPath() + "/config/config.json";

    //4. 确保config目录存在
    QFileInfo fileInfo(configPath);
    QDir dir = fileInfo.absoluteDir();
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    //5. 写入文件
    QFile file(configPath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        LOG_WARNING() << "无法保存配置文件：" << configPath;
        return;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO() << "保存配置文件成功";
    LOG_INFO() << "配置文件路径：" << configPath;

}
