#ifndef CONTROLDELEGATE_H
#define CONTROLDELEGATE_H

#include <QObject>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QItemDelegate>
#include <QApplication>
#include <QStyledItemDelegate>
#include <QStyleOption>
#include <QMouseEvent>
#include <QPainter>

class SpinboxDelegate : public QItemDelegate
{
        Q_OBJECT

public:
    SpinboxDelegate(int column);
    //重写QItemDelegate里的函数
    //const修饰引用传递，"引用传递"仅借用一下参数的别名，不需要产生临时对象,所以可以提高效率.使用const可以确保引用的参数不被修改
    //const加在最后,函数的数据成员mColumn不可被修改
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //void setEditorData(QWidget *editor, const QModelIndex &index) const ;       //为editor提供编辑的原始数据
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const ;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;     //保证editor显示在 item view 的合适位置以及大小

private slots:
    void commitAndCloseEditor();

private:
    int mColumn;
};

class CheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    CheckBoxDelegate(QObject *parent = 0);
protected:
    void paint(QPainter* painter,const QStyleOptionViewItem& option,const QModelIndex& index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);
};

class ComboboxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ComboboxDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();
};

#endif // CONTROLDELEGATE_H
