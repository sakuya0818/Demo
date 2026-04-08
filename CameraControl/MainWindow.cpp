#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 确保所有线程都停止后再关闭
    qDebug() << "正在关闭程序...";

    // 给线程一些时间清理资源
    QThread::msleep(100);

    // 接受关闭事件
    event->accept();
}
