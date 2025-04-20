#ifndef NAVIGATIONLIST_H
#define NAVIGATIONLIST_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class NavigationList; }
QT_END_NAMESPACE

class NavigationList : public QWidget
{
    Q_OBJECT
public:
    explicit NavigationList(QWidget *parent = nullptr);
    ~NavigationList();
    void initTreeView();

protected:
    void paintEvent(QPaintEvent* _event) override;

public slots:
    void slotListViewPressed(const QModelIndex &);

signals:
    void signalPageSwitch(QString page);      // 页面切换信号

private:
    Ui::NavigationList *ui;
    bool m_isHideAdditional = true;
};
#endif // NAVIGATIONLIST_H
