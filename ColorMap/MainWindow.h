#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CustomPlot/qcustomplot.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 初始化界面
    void initWindow();

private slots:
    void on_Btn_Clear_clicked();

    void on_Btn_AddText_clicked();

    void on_Btn_AddEllipse_clicked();

    void on_Btn_AddLine_clicked();

private:
    Ui::MainWindow *ui;

    QVector<QCPItemText*> m_itemText;           // 文字
    QVector<QCPItemEllipse*> m_itemEllipse;     // 圆
    QVector<QCPItemLine*> m_itemLine;           // 线
};
#endif // MAINWINDOW_H
