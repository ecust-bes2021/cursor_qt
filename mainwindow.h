#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QCoreApplication>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onBtnGetFilePathClicked();

private:
    Ui::MainWindow *ui;   

private:
    void loadConfig();
    void saveConfig();
};
#endif // MAINWINDOW_H
