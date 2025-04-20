#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tableViewInit();

    OperationMenu = new QMenu();
    DeleteAction = new QAction("删除");
    InsertAction = new QAction("插入");
    OperationMenu->addAction(DeleteAction);
    OperationMenu->addAction(InsertAction);

    connect(InsertAction, SIGNAL(triggered(bool)), this, SLOT(sltInsertAction()));
    connect(DeleteAction, SIGNAL(triggered(bool)), this, SLOT(sltDeleteAction()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) //重写上下文事件
{
    //方法一
    if(ui->tableView->hasFocus())
    {
        OperationMenu->move(cursor().pos());
        OperationMenu->show();
    }
    //方法二
//    {
//        ui->tableView->addAction(InsertAction);
//        ui->tableView->addAction(DeleteAction);
//        ui->tableView->(Qt::ActionsContextMenu);
//    }
}

void MainWindow::tableViewInit()
{
    tableModel = new QStandardItemModel;
    ui->tableView->setModel(tableModel);

    tableModel->setHorizontalHeaderItem(0, new QStandardItem("学号"));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem("姓名"));
    tableModel->setHorizontalHeaderItem(2, new QStandardItem("性别"));
    tableModel->setHorizontalHeaderItem(3, new QStandardItem("语文"));
    tableModel->setHorizontalHeaderItem(4, new QStandardItem("数学"));
    tableModel->setHorizontalHeaderItem(5, new QStandardItem("测试通过"));

    ui->tableView->setModel(tableModel);

    //tableView委托设置每一列里的控件
    ui->tableView->setItemDelegateForColumn(2, new ComboboxDelegate());
    ui->tableView->setItemDelegateForColumn(3, new SpinboxDelegate(3));
    ui->tableView->setItemDelegateForColumn(4, new SpinboxDelegate(4));
    ui->tableView->setItemDelegateForColumn(5, new CheckBoxDelegate());

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//插入行操作
void MainWindow::sltInsertAction()
{
    QList<QStandardItem*> item;
    item.append(new QStandardItem());
    item.append(new QStandardItem());
    item.append(new QStandardItem());
    item.append(new QStandardItem());
    item.append(new QStandardItem());
    item.append(new QStandardItem());
    //设置列数的对齐方式
    item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tableModel->insertRow(tableModel->rowCount(), item);
}

//删除指定行操作
void MainWindow::sltDeleteAction()
{
    int curreantRow = ui->tableView->currentIndex().row();                    //获取当前行数
    tableModel->removeRow(curreantRow);
}
