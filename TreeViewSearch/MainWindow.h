#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitTreeView(QString path);

    bool GetAllFiles(QString path, QStandardItem *parentItem);

    void GetItem(QStandardItem *item);

    void ParentExpand(QStandardItem *item);

private slots:
    void on_Btn_Search_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *m_treeViewModel;
};
#endif // MAINWINDOW_H
