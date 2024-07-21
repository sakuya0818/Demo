#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xvideothread.h"
#include "QMessageBox"
#include "QString"

QString url="rtsp://admin:HK123456@192.168.1.64/h264/ch33/sub/av_stream";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/Resource/heart.ico"));
    ui->lineEdit_url->setText(url);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//开始连接
void MainWindow::on_pushButton_link_clicked()
{
    QString url=ui->lineEdit_url->text();
    if(url.isEmpty())
    {
        QMessageBox::warning(this,"警告","当前输入地址为空");
    }else{
        vt=new xVideoThread();
        vt->open(url,ui->label_video);
    }
}
