#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialPort;
}

class SerialPort : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = 0);
    ~SerialPort();

    void dragEnterEvent(QDragEnterEvent *event);     //进入拖动事件

    void dropEvent(QDropEvent *event);

    void foreachSerial();

    void loadTXT(QString filePath);

    void StringToHex(QString str, QByteArray &senddata);

    char ConvertHexChar(char ch);

private slots:
    void on_Btn_Send_clicked();

    void ReadData();

    void on_Btn_Connect_clicked();

    void on_Btn_Search_clicked();

    void on_Btn_Clear_clicked();

private:
    Ui::SerialPort *ui;

    QSerialPort *serial;

    bool serialExist;

    //保存comcoBox中的数值在设置参数时使用
    int m_serialportbaudrate;
    int m_serialportdatabit;
    int m_serialportstopbit;
    int m_serialportparitybit;
};

#endif // SERIALPORT_H
