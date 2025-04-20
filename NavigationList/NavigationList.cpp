#include "NavigationList.h"
#include "ui_NavigationList.h"

NavigationList::NavigationList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationList)
{
    ui->setupUi(this);
    initTreeView();

    connect(ui->listViewNavigation, &NavListView::pressed, this, &NavigationList::slotListViewPressed);
    ui->listViewNavigation->setCurrentRow(0);
}

NavigationList::~NavigationList()
{
    delete ui;
}

void NavigationList::paintEvent(QPaintEvent* _event)
{
    Q_UNUSED(_event)
    QStyleOption n_styleOption;
    n_styleOption.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &n_styleOption, &painter, this);
}

void NavigationList::initTreeView()
{
    ui->listViewNavigation->setIcoColorBg(false);
    ui->listViewNavigation->setColorLine(QColor("#FFFFFF"));
    ui->listViewNavigation->setColorBg(QColor("#016BFF"),
                                          QColor("#2A83FF"),
                                          QColor("#2A83FF"));
    ui->listViewNavigation->setColorText(QColor("#FFFFFF"),
                                            QColor("#FFFFFF"),
                                            QColor(0, 0, 0));
    ui->listViewNavigation->setColorBgLeval2(QColor("#EBF1FF"),QColor("#EBF1FF"),QColor("#EBF1FF"));
    ui->listViewNavigation->setColorTextLeval2(QColor("#000000"),
                                                  QColor("#000000"),
                                                  QColor("#6D6D6D"));
    // 设置数据方式
    QStringList listItem;
    listItem.append(QString::fromLocal8Bit("Tab1||0||:/Images/1.png|"));
    listItem.append(QString::fromLocal8Bit("Tab2||0||:/Images/2.png|"));
    listItem.append(QString::fromLocal8Bit("Tab3||1||:/Images/3.png|"));
    listItem.append(QString::fromLocal8Bit("Tab4|Tab3|||:/Images/4.png|"));
    listItem.append(QString::fromLocal8Bit("Tab5|Tab3|||:/Images/5.png|"));
    listItem.append(QString::fromLocal8Bit("Tab6|Tab3|||:/Images/6.png|"));
    listItem.append(QString::fromLocal8Bit("Tab7|Tab3|||:/Images/7.png|"));
    listItem.append(QString::fromLocal8Bit("Tab8||0||:/Images/8.png|"));
    listItem.append(QString::fromLocal8Bit("Tab9||0||:/Images/9.png|"));
    ui->listViewNavigation->setData(listItem);
}

void NavigationList::slotListViewPressed(const QModelIndex &)
{
    // 获取到点击的某一行，再根据点击显示对应的界面
    QModelIndex index = ui->listViewNavigation->currentIndex();
    QString text = index.data().toString();
    emit signalPageSwitch(text);
}

