#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/Resouce/heart.ico"));

    initWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWindow()
{
    // 配置轴矩形，允许通过拖拽/缩放尺度改变颜色范围
    ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectPlottables);
    ui->customPlot->axisRect()->setupFullAxesBox(true);

    // 设置x轴和y轴的坐标轴名字
    ui->customPlot->xAxis->setLabel("横向距离(m)");
    ui->customPlot->yAxis->setLabel("纵向距离(m)");

    // 创建QCPColorMap对象
    QCPColorMap *colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
    colorMap->setAntialiased(true);
    // 设置QCPColorMap的数据点为 nx*ny 个
    int nx = 257;
    int ny = 257;
    colorMap->data()->setSize(nx, ny);
    // 设置X轴和Y轴的坐标范围
    colorMap->data()->setRange(QCPRange(-3, 3), QCPRange(-3, 3));

    // 打开文件，读取数据
    QFile file(":/Resouce/data.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QList<double> data;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        double a = line.toDouble();
        data.push_back(a);
    }

    // 根据读到的数据，为QCPColorMap的每个单元格点设置数据
    double x, y, z;
    for (int xIndex = 0; xIndex < nx; ++xIndex)
    {
        for (int yIndex = 0; yIndex < ny; ++yIndex)
        {
            // 把xIndex和yIndex表示的单元格索引，转换为此QCPColorMapData的单元格索引
            colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
            // 把读到的数据值赋值给QCPColorMap的每个单元格
            z = data.at(xIndex * 257 + yIndex);
            colorMap->data()->setCell(xIndex, yIndex, z);
        }
    }

    // 添加右侧的色带
    QCPColorScale *colorScale = new QCPColorScale(ui->customPlot);
    // 设置色带值的范围
    colorScale->setDataRange(QCPRange(-190, 0));
    // 将颜色图与色标关联
    colorMap->setColorScale(colorScale);
    // 设置色带刻度垂直条，刻度线/坐标轴标签右侧（实际上，右侧已经是默认值）
    colorScale->setType(QCPAxis::atRight);
    // 设置色带右侧的名字
    colorScale->axis()->setLabel("Strength Value");
    // 将其添加到主轴矩形的右侧
    ui->customPlot->plotLayout()->addElement(0, 1, colorScale);
    // 使用插值，界面不显示小方块(默认是禁用的)
    colorMap->setInterpolate(true);
    // 将颜色贴图的“颜色渐变”设置为其中一个预设
    colorMap->setGradient(QCPColorGradient::gpJet);
    // 确保轴rect和色标同步其底边距和顶边距，以便它们对齐
    QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->customPlot);
    ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // 重新缩放数据维度（颜色），以使所有数据点都位于颜色渐变显示的范围内(由于已经使用setDataRange主动设置了色带的值范围，两个选一个就行)
//    colorMap->rescaleDataRange();

    // 重新缩放x轴和y轴，以便可以看到整个颜色图
    ui->customPlot->rescaleAxes();

    ui->customPlot->replot();
}

void MainWindow::on_Btn_Clear_clicked()
{
    // 清除文字、圆和线段
    for (int i = 0; i < m_itemText.size(); ++i)
    {
        if (m_itemText[i] == nullptr)
            continue;
        ui->customPlot->removeItem(m_itemText[i]);
        m_itemText[i] = nullptr;
    }
    for (int i = 0; i < m_itemEllipse.size(); ++i)
    {
        if (m_itemEllipse[i] == nullptr)
            continue;
        ui->customPlot->removeItem(m_itemEllipse[i]);
        m_itemEllipse[i] = nullptr;
    }
    for (int i = 0; i < m_itemLine.size(); ++i)
    {
        if (m_itemLine[i] == nullptr)
            continue;
        ui->customPlot->removeItem(m_itemLine[i]);
        m_itemLine[i] = nullptr;
    }

    ui->customPlot->clearPlottables();
    ui->customPlot->replot();
}

void MainWindow::on_Btn_AddText_clicked()
{
    m_itemText.resize(2);
    QList<QPoint> list;
    list << QPoint(1.2, 1.8) << QPoint(-1.2, -1.8);
    for (int i = 0; i < 2; ++i)
    {
        m_itemText[i] = new QCPItemText(ui->customPlot);
        m_itemText[i]->setText("S" + QString::number(i));
        m_itemText[i]->position->setType(QCPItemPosition::ptPlotCoords); // 位置类型
        m_itemText[i]->position->setCoords(list[i].x(), list[i].y());
        m_itemText[i]->setFont(QFont(font().family(), 12)); // 字体大小
        m_itemText[i]->setColor(Qt::black);
    }

    ui->customPlot->replot();
}

void MainWindow::on_Btn_AddEllipse_clicked()
{
    m_itemEllipse.resize(2);
    QList<QPoint> list;
    list << QPoint(1.2, 1.8) << QPoint(-1.2, -1.8);
    for (int i = 0; i < 2; ++i)
    {
        m_itemEllipse[i] = new QCPItemEllipse(ui->customPlot);
        m_itemEllipse[i]->setAntialiased(true);
        double r = 0.2;//每个通道的圆半径大小
        m_itemEllipse[i]->topLeft->setCoords(list[i].x() - r, list[i].y() + r);
        m_itemEllipse[i]->bottomRight->setCoords(list[i].x() + r, list[i].y() - r);
        m_itemEllipse[i]->setPen(QPen(Qt::black, 1));
    }

    ui->customPlot->replot();
}

void MainWindow::on_Btn_AddLine_clicked()
{
    m_itemLine.resize(2);
    QList<QPoint> list;
    list << QPoint(1.2, 1.8) << QPoint(-1.2, -1.8);
    for (int i = 0; i < 2; ++i)
    {
        m_itemLine[i] = new QCPItemLine(ui->customPlot);
        m_itemLine[i]->start->setCoords(list[i].x() - 0.2, list[i].y());
        m_itemLine[i]->end->setCoords(list[i].x() + 0.2, list[i].y());
        m_itemLine[i]->setPen(QPen(Qt::black, 1));
    }

    ui->customPlot->replot();
}

