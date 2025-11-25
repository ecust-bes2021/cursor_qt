#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //配置处理
    

    //slots
    connect(ui->btn_get_file_path, &QPushButton::clicked, this, &MainWindow::onBtnGetFilePathClicked);
    connect(ui->btn_get_file_path_2, &QPushButton::clicked, this, &MainWindow::onBtnGetFilePathClicked);
    connect(ui->btn_get_file_path_3, &QPushButton::clicked, this, &MainWindow::onBtnGetFilePathClicked);
}

MainWindow::~MainWindow()
{
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
