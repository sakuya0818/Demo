#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化TcpSocket
    socket = new QTcpSocket();
    //取消原有连接
    socket->abort();
}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}

void MainWindow::on_Btn_Connect_clicked()
{
    if(ui->Btn_Connect->text() == tr("连接") && socket->state() != QTcpSocket::ConnectedState )
    {
        //获取IP地址
        QString IP = ui->lineEdit_IP->text();
        //获取端口号
        int port = ui->lineEdit_Port->text().toInt();

        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::Read_Data);
        connect(socket, &QTcpSocket::stateChanged, this, &MainWindow::onStateChanged);
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onErrorOccurred()));

        //取消原有连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(3000))
        {
            return;
        }
        else {
            ui->Btn_Connect->setText("断开\n连接");

            QMessageBox::information(this, "提示", "连接成功", QMessageBox::Yes);
        }
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->Btn_Connect->setText("连接");
        return;
    }
}

void MainWindow::onStateChanged(int state)
{
    if (state == QTcpSocket::UnconnectedState) {
        ui->Btn_send->setEnabled(false);
        ui->Btn_Connect->setText("连接");
    }
    else if (state == QTcpSocket::ConnectedState) {
        ui->Btn_send->setEnabled(true);
        ui->Btn_Connect->setText("断开连接");
    }
}

void MainWindow::onErrorOccurred()
{
    QMessageBox::information(this, "错误", socket->errorString(), QMessageBox::Yes);
}

void MainWindow::Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    //qDebug() << buffer;

    if(!buffer.isEmpty())
    {
        QMessageBox::information(this, "收到消息", buffer, QMessageBox::Yes);
    }
}

void MainWindow::on_Btn_exit_clicked()
{
    this->close();
}

void MainWindow::on_Btn_send_clicked()
{
    QString data = ui->lineEdit_Send->text();
    socket->write(data.toLatin1());
}
