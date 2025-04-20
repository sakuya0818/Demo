#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "controldelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void tableViewInit();

private slots:
    void contextMenuEvent(QContextMenuEvent *event);

    //插入行操作
    void sltInsertAction();

    //删除指定行操作
    void sltDeleteAction();

private:
    Ui::MainWindow *ui;

    QMenu *OperationMenu;                   //操作菜单
    QAction *DeleteAction;                  //删除动作
    QAction *InsertAction;                  //插入动作

    QStandardItemModel *tableModel;
};

#endif // MAINWINDOW_H
