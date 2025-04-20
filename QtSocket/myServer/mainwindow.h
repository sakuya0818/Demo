#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpserver>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Btn_Listen_clicked();

    void server_New_Connect();

    void Read_Data();

    void disConnected();

    void on_Btn_Send_clicked();

    void on_Btn_Clean_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *socket;
    QTcpServer *server;

    QList<QTcpSocket*> clientSocket;
};

#endif // MAINWINDOW_H
