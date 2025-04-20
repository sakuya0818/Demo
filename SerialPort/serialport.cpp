#include "serialport.h"
#include "ui_serialport.h"

#include <QButtonGroup>

SerialPort::SerialPort(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);

    this->setFixedSize(670, 470);

    //设置按钮图标
    ui->Btn_Search->setIcon(QIcon(":/icon/icon/search.png"));
    ui->Btn_Connect->setIcon(QIcon(":/icon/icon/connect.png"));
    ui->Btn_Send->setIcon(QIcon(":/icon/icon/send.png"));
    ui->Btn_Clear->setIcon(QIcon(":/icon/icon/clear.png"));

    //设置只有一个checkbox可以点击
    QButtonGroup* sendCheckGroup = new QButtonGroup(this);
    sendCheckGroup->addButton(ui->checkBox_SendHex, 1);
    sendCheckGroup->addButton(ui->checkBox_SendChr, 2);
    QButtonGroup* RecCheckGroup = new QButtonGroup(this);
    RecCheckGroup->addButton(ui->checkBox_RecHex, 1);
    RecCheckGroup->addButton(ui->checkBox_RecChr, 2);

    //设置textEdit默认带滚动条和只可读
    ui->textEdit_Rec->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->textEdit_Rec->setReadOnly(true);
    ui->textEdit_Rec->setAcceptDrops(false);
    ui->textEdit_Send->setAcceptDrops(false);
    this->setAcceptDrops(true);

    //查找可用串口
    serialExist = false;
    foreachSerial();
}

SerialPort::~SerialPort()
{
    delete ui;
}

void SerialPort::dragEnterEvent(QDragEnterEvent *event)
{
    //如果类型是txt才接受拖动。
    if (event->mimeData()->hasFormat("text/uri-list"))
            event->acceptProposedAction();
    else
       event->ignore();//否则不接受鼠标事件
}

void SerialPort::dropEvent(QDropEvent *event)
{
    //获取MIMEData
    const QMimeData*mime=event->mimeData();
    QString filePath = mime->urls()[0].toLocalFile();
    if (filePath.isEmpty()) {
        return;
    }
    else {
        loadTXT(filePath);
    }
}

void SerialPort::loadTXT(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
         QTextStream in(&file);
         while (!in.atEnd())
         {
             QString line = in.readLine();      //整行读取文本
             ui->textEdit_Send->setText(line);
         }
    }
}

void SerialPort::on_Btn_Search_clicked()
{
    foreachSerial();
}

void SerialPort::on_Btn_Connect_clicked()
{
    if (serialExist) {
        if(ui->Btn_Connect->text() == tr("打开串口")) {
            serial = new QSerialPort;
            //设置串口名
            serial->setPortName(ui->comboBox_SerialNumber->currentText());
            //打开串口
            serial->open(QIODevice::ReadWrite);
            //设置波特率
            if (ui->comboBox_BaudRate->currentText() == "1200")
                serial->setBaudRate(QSerialPort::Baud1200);
            else if (ui->comboBox_BaudRate->currentText() == "2400")
                serial->setBaudRate(QSerialPort::Baud2400);
            else if (ui->comboBox_BaudRate->currentText() == "4800")
                serial->setBaudRate(QSerialPort::Baud4800);
            else if (ui->comboBox_BaudRate->currentText() == "9600")
                serial->setBaudRate(QSerialPort::Baud9600);
            else if (ui->comboBox_BaudRate->currentText() == "19200")
                serial->setBaudRate(QSerialPort::Baud19200);
            else if (ui->comboBox_BaudRate->currentText() == "38400")
                serial->setBaudRate(QSerialPort::Baud38400);
            else if (ui->comboBox_BaudRate->currentText() == "57600")
                serial->setBaudRate(QSerialPort::Baud57600);
            else if (ui->comboBox_BaudRate->currentText() == "115200")
                serial->setBaudRate(QSerialPort::Baud115200);
            //设置校验位
            if (ui->comboBox_ParityBit->currentIndex() == 0)
                serial->setParity(QSerialPort::NoParity);
            else if (ui->comboBox_ParityBit->currentIndex() == 1)
                serial->setParity(QSerialPort::OddParity);
            else if (ui->comboBox_ParityBit->currentIndex() == 2)
                serial->setParity(QSerialPort::EvenParity);
            //设置数据位
            if (ui->comboBox_DataBit->currentText() == "8")
                serial->setDataBits(QSerialPort::Data8);
            else if (ui->comboBox_DataBit->currentText() == "7")
                serial->setDataBits(QSerialPort::Data7);
            else if (ui->comboBox_DataBit->currentText() == "6")
                serial->setDataBits(QSerialPort::Data6);
            else if (ui->comboBox_DataBit->currentText() == "5")
                serial->setDataBits(QSerialPort::Data5);
            //设置停止位
            if (ui->comboBox_StopBit->currentText() == "1")
                serial->setStopBits(QSerialPort::OneStop);
            else if (ui->comboBox_StopBit->currentText() == "1.5")
                serial->setStopBits(QSerialPort::OneAndHalfStop);
            else if (ui->comboBox_StopBit->currentText() == "2")
                serial->setStopBits(QSerialPort::TwoStop);
            //设置流控制
            if (ui->comboBox_FlowContral->currentText() == "off")
                serial->setFlowControl(QSerialPort::NoFlowControl);
            else if (ui->comboBox_FlowContral->currentText() == "hardware")
                serial->setFlowControl(QSerialPort::HardwareControl);
            else if (ui->comboBox_FlowContral->currentText() == "xonxoff")
                serial->setFlowControl(QSerialPort::SoftwareControl);

            //关闭设置菜单使能
            ui->comboBox_BaudRate->setEnabled(false);
            ui->comboBox_DataBit->setEnabled(false);
            ui->comboBox_ParityBit->setEnabled(false);
            ui->comboBox_SerialNumber->setEnabled(false);
            ui->comboBox_StopBit->setEnabled(false);
            ui->Btn_Connect->setText("关闭串口");

            //连接信号槽
            connect(serial, &QSerialPort::readyRead, this, &SerialPort::ReadData);
        }
        else {
            //关闭串口
            serial->clear();
            serial->close();
            serial->deleteLater();

            //恢复菜单设置使能
            ui->comboBox_BaudRate->setEnabled(true);
            ui->comboBox_DataBit->setEnabled(true);
            ui->comboBox_ParityBit->setEnabled(true);
            ui->comboBox_SerialNumber->setEnabled(true);
            ui->comboBox_StopBit->setEnabled(true);
            ui->Btn_Connect->setText("打开串口");
        }
    }
}

//查找串口
void SerialPort::foreachSerial()
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_SerialNumber->addItem(serial.portName());
            serial.close();
            serialExist = true;
        }
    }
    if(!serialExist)
    {
        QMessageBox::information(this, tr("提示"), tr("计算机上没有可用串口"), QMessageBox::Yes);
    }
}

//读取接收到的信息
void SerialPort::ReadData()
{
    QByteArray buffer;
    buffer = serial->readAll();
    if (!buffer.isEmpty()) {
        QString data;
        if (ui->checkBox_RecChr->isChecked()) {
            data = buffer;
        }
        else if (ui->checkBox_RecHex->isChecked()) {
#if 1
            QDataStream out(&buffer, QIODevice::ReadWrite);               //将字节数组读入
            while(!out.atEnd())
            {
                qint8 outChar = 0;
                out >> outChar;                                           //每字节填充一次，直到结束
                QString temp = QString("%1").arg(outChar&0xFF, 2, 16, QLatin1Char('0')).toUpper() + " ";       //十六进制的转换
                data += temp;
            }
#else
            for(int i = 0; i < buffer.count(); i++){
                QString temp;
                temp.sprintf("0x%02x, ", (unsigned char)buffer.at(i));
                data += temp;
            }
#endif
        }
        ui->textEdit_Rec->append(data);
    }
    buffer.clear();
}

//发送按钮
void SerialPort::on_Btn_Send_clicked()
{
    QString sendContent = ui->textEdit_Send->toPlainText();

    if (ui->checkBox_SendChr->isChecked()) {
        QByteArray m_sendContent = sendContent.toUtf8();
        if (!m_sendContent.isEmpty() && !m_sendContent.isNull()) {
            serial->write(m_sendContent);
        }
    }
    else if (ui->checkBox_SendHex->isChecked()){
        //如果发送的数据个数为奇数，则在前面最后的单个字符前添加一个字符0
        if (sendContent.length() % 2){
            sendContent.insert(sendContent.length()-1, '0');
        }
        QByteArray m_sendContent;
        StringToHex(sendContent, m_sendContent);
        serial->write(m_sendContent);
    }
}

void SerialPort::on_Btn_Clear_clicked()
{
    ui->textEdit_Rec->clear();
}

void SerialPort::StringToHex(QString str, QByteArray &senddata) //字符串转换为十六进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len / 2);
    char lstr,hstr;
    for (int i = 0; i < len; ) {
        hstr = str[i].toLatin1();
        if (hstr == ' ') {
            ++i;
            continue;
        }
        ++i;
        if (i  >= len) break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if ((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16 + lowhexdata;
        ++i;
        senddata[hexdatalen] = (char)hexdata;
        ++hexdatalen;
    }
    senddata.resize(hexdatalen);
}

char SerialPort::ConvertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - 0x30;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch - 'A' + 10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else return ch -  ch;
}
