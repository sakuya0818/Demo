#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/Resouce/heart.ico"));

    // 设置树形结构
    m_treeViewModel = new QStandardItemModel(ui->treeView);
    m_treeViewModel->setHorizontalHeaderLabels(QStringList() << "文件名称" << "文件路径");
    ui->treeView->setModel(m_treeViewModel);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->hideColumn(1);
    ui->treeView->header()->hide();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    InitTreeView("E:/MyWorkSpace/Qt/Demo");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitTreeView(QString path)
{
    QFileInfo fileInfo(path);
    if (!fileInfo.isDir())
    {
        return;
    }

    m_treeViewModel->removeRows(0, m_treeViewModel->rowCount());
    // 绑定文件夹的图标和文件路径
    QStandardItem *item = new QStandardItem(QIcon(":/Resouce/Folder.png"), path.split("/").last());
    m_treeViewModel->appendRow(item);
    m_treeViewModel->setItem(m_treeViewModel->indexFromItem(item).row(), 1, new QStandardItem(fileInfo.path()));

    GetAllFiles(path, item);
}

bool MainWindow::GetAllFiles(QString path, QStandardItem *parentItem)
{
    QDir sourceDir(path);
    // 文件按时间排序
    QStringList filter;
    sourceDir.setNameFilters(filter);
    QList<QFileInfo> fileInfoList = QList<QFileInfo>(sourceDir.entryInfoList(filter, QDir::AllEntries, QDir::Time));
    // 遍历当前文件夹下的文件列表
    foreach (QFileInfo fileInfo, fileInfoList)
    {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        if (fileInfo.isDir())
        {
            // 如果存在子级，添加子级数据
            QStandardItem *item = new QStandardItem(QIcon(":/Resouce/Folder.png"), fileInfo.baseName());
            parentItem->appendRow(item);
            parentItem->setChild(item->row(), 1, new QStandardItem(fileInfo.filePath()));
            GetAllFiles(fileInfo.filePath(), item);
        }
        else if (fileInfo.isFile())
        {
            // 如果存在子级，添加子级数据
            QStandardItem *item = new QStandardItem(QIcon(":/Resouce/Document.png"), fileInfo.baseName());
            parentItem->appendRow(item);
            parentItem->setChild(item->row(), 1, new QStandardItem(fileInfo.filePath()));
        }
    }

    return true;
}

void MainWindow::GetItem(QStandardItem *item)
{
    Q_ASSERT(item);

    // 如果该行包含要查找的字符串，展开所有的父节点并把背景设为红色
    if (item->text().contains(ui->lineEdit->text()))
    {
        item->setBackground(QBrush(qRgb(255, 125, 125)));
        ParentExpand(item);
    }
    else
    {
        item->setBackground(QBrush(qRgb(255, 255, 255)));
    }

    // 如果有子项，继续查找所有子项
    if (item->hasChildren())
    {
        for (int i = 0; i < item->rowCount(); i++)
        {
            item->setSelectable(true);
            QStandardItem *childItem = item->child(i);
            GetItem(childItem);
        }
    }
}

void MainWindow::ParentExpand(QStandardItem *item)
{
    // 如果有父节点，就展开
    if (item->parent() != nullptr)
    {
        QStandardItem *parentItem = item->parent();
        ui->treeView->expand(parentItem->index());
        ParentExpand(parentItem);
    }
}

void MainWindow::on_Btn_Search_clicked()
{
    // 所有行全部折叠
    ui->treeView->collapseAll();

    if (!ui->lineEdit->text().isEmpty())
    {
        // 遍历查找所有行
        for (int i = 0; i < m_treeViewModel->rowCount(); i++)
        {
            GetItem(m_treeViewModel->item(i));
        }
    }
    else
    {
        for (int i = 0; i < m_treeViewModel->rowCount(); i++)
        {
            m_treeViewModel->item(i)->setBackground(QBrush(qRgb(255, 255, 255)));
        }
    }
}
