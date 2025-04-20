#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();
    server = new QTcpServer();

    // 获取本地的IP
    for (int i = 0; i < QNetworkInterface().allAddresses().length(); ++i) {
        ui->comboBox_IP->addItem(QNetworkInterface().allAddresses().at(i).toString());
    }

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::blue);
    ui->label_SeverState->setPalette(pe);
    ui->label_SeverState->setText("服务器未打开");

    // 设置tableWidget不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 关联客户端连接信号newConnection
    connect(server, &QTcpServer::newConnection, this, &MainWindow::server_New_Connect);
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_Btn_Listen_clicked()
{
    if (ui->Btn_Listen->text() == tr("侦听")) {
        // 从输入端获取端口号
        int port = ui->label_Port->text().toInt();

        // 侦听指定的端口
        if(!server->listen(QHostAddress::Any, port)) {
            // 若出错，则输出错误信息
            QMessageBox::information(this, tr("错误"), server->errorString(), QMessageBox::Yes);
            return;
        }
        else {
            // 修改按键文字
            ui->Btn_Listen->setText("取消侦听");
            QPalette pe;
            pe.setColor(QPalette::WindowText, Qt::red);
            ui->label_SeverState->setPalette(pe);
            ui->label_SeverState->setText("服务器运行中...");
        }
    }
    else {
        // 如果正在连接......
        if(socket->state() == QAbstractSocket::ConnectedState) {
            // 关闭连接
            socket->disconnectFromHost();
        }
        // 取消侦听
        server->close();
        // 修改按键文字
        ui->Btn_Listen->setText("侦听");
        QPalette pe;
        pe.setColor(QPalette::WindowText, Qt::blue);
        ui->label_SeverState->setPalette(pe);
        ui->label_SeverState->setText("服务器未打开");
    }
}

void MainWindow::server_New_Connect()
{
    // 获取客户端连接
    socket = server->nextPendingConnection();
    clientSocket.append(socket);

    // 把连接到的客户端添加入tableWidget中
    int currentRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(currentRow);
    QTableWidgetItem *item = new QTableWidgetItem();
    QTableWidgetItem *item_2 = new QTableWidgetItem();
    QTableWidgetItem *item_3 = new QTableWidgetItem();
    QTableWidgetItem *item_4 = new QTableWidgetItem();
    item->setText(tr("0000000%1").arg(QString::number(ui->tableWidget->rowCount())));
    item_2->setText(clientSocket[currentRow]->peerAddress().toString().mid(7));
    item_3->setText(QString::number(clientSocket[currentRow]->peerPort()));
    item_4->setText("在线");
    ui->tableWidget->setItem(currentRow, 0, item);
    ui->tableWidget->setItem(currentRow, 1, item_2);
    ui->tableWidget->setItem(currentRow, 2, item_3);
    ui->tableWidget->setItem(currentRow, 3, item_4);

    // 连接QTcpSocket的信号槽，以读取新数据
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read_Data()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disConnected()));
}

// 从客户端接收到的消息
void MainWindow::Read_Data()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for (int i = 0; i < clientSocket.length(); ++i) {
        // 读取缓冲区数据
        QByteArray buffer = clientSocket[i]->readAll();
        if(buffer.isEmpty()) {
            continue;
        }

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(clientSocket[i]->peerAddress().toString().mid(7)).arg(clientSocket[i]->peerPort());

        // 若此次消息的地址与上次不同，则需显示此次消息的客户端地址
        if (IP_Port != IP_Port_Pre) {
            ui->textEdit_Recv->append(IP_Port);
        }

        ui->textEdit_Recv->append(buffer);

        // 更新ip_port
        IP_Port_Pre = IP_Port;
    }
}

void MainWindow::disConnected()
{
    // 遍历寻找断开连接的是哪一个客户端
    for(int i = 0; i < clientSocket.length(); ++i) {
        if(clientSocket[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tableWidget中的该客户端信息
            for (int j = 0; j < ui->tableWidget->rowCount(); ++j) {
                if (clientSocket[i]->peerAddress().toString().mid(7) == ui->tableWidget->item(j, 1)->text()) {
                    ui->tableWidget->removeRow(j);
                }
            }
            // 删除存储在clientSocket列表中的客户端信息
            clientSocket[i]->destroyed();
            clientSocket.removeAt(i);
        }
    }
}

void MainWindow::on_Btn_Send_clicked()
{
    // 获取选中的行
    int row = ui->tableWidget->currentRow();

    QString data = ui->lineEdit_send->text();
    if (data.isEmpty()) {
        QMessageBox::information(this, "提示", "请输入发送内容！", QMessageBox::Yes);
    }
    else {
        if (row >= 0) {
            for (int i = 0; i < clientSocket.length(); ++i) {
                if (QString::number(clientSocket[i]->peerPort()) == ui->tableWidget->item(row, 2)->text()) {
                    //以ASCII码形式发送文本框内容
                    clientSocket[i]->write(data.toLatin1());
                }
            }
        }
        else {
            socket->write(data.toLatin1());
        }
    }
}

void MainWindow::on_Btn_Clean_clicked()
{
    ui->textEdit_Recv->clear();
}
