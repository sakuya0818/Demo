#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xvideothread.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_link_clicked();

private:
    Ui::MainWindow *ui;
    xVideoThread *vt;
};
#endif // MAINWINDOW_H
