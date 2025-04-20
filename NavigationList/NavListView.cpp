#include "NavListView.h"

#include <QPainter>
#include <QFile>
#include <qdom.h>
#include <QDebug>

NavDelegate::NavDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    nav = (NavListView *)parent;
}

NavDelegate::~NavDelegate()
{

}

QSize NavDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NavModel::TreeNode *node = (NavModel::TreeNode *)index.data(Qt::UserRole).toULongLong();
    if (node->level == 1)
    {
        return QSize(192, 71);
    }
    else
    {
        return QSize(182, 48);
    }
}

void NavDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    NavModel::TreeNode *node = (NavModel::TreeNode *)index.data(Qt::UserRole).toULongLong();

    QColor colorBg;
    QColor colorText;
    QFont  fontText;
    int    iconSize = 0, leftMargin = 0, topMargin = 0;
    if(1 == node->level)
    {
        if (option.state & QStyle::State_Selected)
        {
            colorBg   = nav->getColorBgSelected();
            colorText = nav->getColorTextSelected();
        }
        else if (option.state & QStyle::State_MouseOver)
        {
            colorBg   = nav->getColorBgHover();
            colorText = nav->getColorTextHover();
        }
        else
        {
            colorBg   = nav->getColorBgNormal();
            colorText = nav->getColorTextNormal();
        }
        iconSize    = 32;
        leftMargin  = 32;
        topMargin   = 20;
        fontText.setPixelSize(20);
        painter->setBrush(QBrush(nav->getColorBgNormal()));
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(option.rect, 8, 20, Qt::RelativeSize);
    }
    else if(2 == node->level)
    {
        if (option.state & QStyle::State_Selected)
        {
            colorBg   = nav->getColorBgSelectedLeval2();
            colorText = nav->getColorTextSelectedLeval2();
        }
        else if (option.state & QStyle::State_MouseOver)
        {
            colorBg = nav->getColorBgHoverLeval2();
            colorText = nav->getColorTextHoverLeval2();
        }
        else
        {
            colorBg   = nav->getColorBgNormalLeval2();
            colorText = nav->getColorTextNormalLeval2();
        }
        iconSize   = 24;
        leftMargin = 25;
        topMargin  = 13;
        fontText.setPixelSize(18);

        QRect rectLevel2 = option.rect;
        rectLevel2.setX(option.rect.x() + 12);
        if (node->theFirst)
        {
            rectLevel2.setHeight(option.rect.height() + 4);
            rectLevel2.setWidth(option.rect.width() + 8);
            painter->setBrush(QBrush(nav->getColorBgNormalLeval2()));
            painter->setPen(Qt::transparent);
            painter->drawRoundedRect(rectLevel2, 8, 20, Qt::RelativeSize);
        }
        else if (node->theLast)
        {
            rectLevel2.setY(option.rect.y() - 4);
            rectLevel2.setWidth(option.rect.width() + 8);
            painter->setBrush(QBrush(nav->getColorBgNormalLeval2()));
            painter->setPen(Qt::transparent);
            painter->drawRoundedRect(rectLevel2, 8, 20, Qt::RelativeSize);
        }
        else
        {
            painter->fillRect(rectLevel2, nav->getColorBgNormalLeval2());
        }
    }

    /// ====== 菜单选项背景颜色
    if (1 == node->level && option.state & QStyle::State_Selected)
    {
        QRect rectMenu = option.rect;
        rectMenu.setWidth(option.rect.width()  - 20);
        rectMenu.setHeight(option.rect.height()- 10);
        rectMenu.setX(option.rect.x() + 10);
        rectMenu.setY(option.rect.y() + 10);
        painter->setBrush(QBrush(colorBg));
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(rectMenu, 8, 20, Qt::RelativeSize);
    }

    /// ====== 绘制图标
    QPixmap pixMap;
    pixMap.load(node->iconName);
    QRect rectIcon = option.rect;
    rectIcon.setX(option.rect.x()+leftMargin);
    rectIcon.setY(option.rect.y()+topMargin);
    rectIcon.setWidth(iconSize);
    rectIcon.setHeight(iconSize);
    painter->drawPixmap(rectIcon, pixMap);

    /// ====== 绘制条目文字
    if(option.state & QStyle::State_Selected)
    {
        painter->setOpacity(1);
    }
    else
    {
        painter->setOpacity(0.5);
    }
    painter->setPen(QPen(colorText));
    int margin = 72;
    if (node->level == 2)
    {
        margin = 84;
    }
    QRect rect = option.rect;
    rect.setX(rect.x() + margin);
    painter->setFont(fontText);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, index.data(Qt::DisplayRole).toString());

    painter->setOpacity(1);
    /// ====== 绘制分割线
    QRect rectLine = option.rect;
    rectLine.setX(option.rect.x()+16);
    rectLine.setY(option.rect.y()-1);
    rectLine.setWidth(168);
    rectLine.setHeight(1);
    QPixmap pixMapLine;
    pixMapLine.load(":/Images/Line.png");
    painter->drawPixmap(rectLine, pixMapLine);
}


NavModel::NavModel(QObject *parent)	: QAbstractListModel(parent)
{

}

NavModel::~NavModel()
{
    for (std::vector<TreeNode *>::iterator it = treeNode.begin(); it != treeNode.end();) {
        for (std::list<TreeNode *>::iterator child = (*it)->children.begin(); child != (*it)->children.end();) {
            delete(*child);
            child = (*it)->children.erase(child);
        }
        delete(*it);
        it = treeNode.erase(it);
    }
}

void NavModel::readData(QString path)
{
    QFile xml(path);

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&xml, false))
    {
        return;
    }

    treeNode.clear();
    listNode.clear();

    QDomNode root = doc.documentElement().firstChildElement("layout");
    QDomNodeList children = root.childNodes();

    for (int i = 0; i != children.count(); ++i)
    {
        QDomElement nodeInfo = children.at(i).toElement();
        TreeNode *node = new TreeNode;
        node->label    = nodeInfo.attribute("label");
        node->collapse = nodeInfo.attribute("collapse").toInt();
        node->info     = nodeInfo.attribute("info");
        node->level    = 1;

        QDomNodeList secondLevel = nodeInfo.childNodes();
        for (int j = 0; j != secondLevel.count(); ++j)
        {
            QDomElement secNodeInfo = secondLevel.at(j).toElement();
            TreeNode *secNode = new TreeNode;
            secNode->label = secNodeInfo.attribute("label");
            secNode->info  = secNodeInfo.attribute("info");
            secNode->collapse = false;
            secNode->level    = 2;
            secNode->theLast  = (j == secondLevel.count() - 1 && i != children.count() - 1);
            node->children.push_back(secNode);
        }

        treeNode.push_back(node);
    }

    refreshList();
    beginResetModel();
    endResetModel();
}

void NavModel::setData(QStringList listItem)
{
    int count = listItem.count();

    if (count == 0) {
        return;
    }
    treeNode.clear();
    listNode.clear();

    // listItem格式: 标题|父节点标题(父节点为空)|是否展开|提示信息
    for (int i = 0; i < count; i++)
    {
        QString item = listItem.at(i);
        QStringList list = item.split("|");

        if (list.count() < 4)
        {
            continue;
        }
        // 首先先将父节点即父节点标题为空的元素加载完毕
        QString title       = list.at(0);
        QString fatherTitle = list.at(1);
        QString collapse    = list.at(2);
        QString info        = list.at(3);
        QString iconFile    = list.at(4);
        if (fatherTitle.isEmpty())
        {
            TreeNode *node = new TreeNode;
            node->label     = title;
            node->collapse  = collapse.toInt();
            node->info      = info;
            node->level     = 1;
            node->iconName  = iconFile;
            // 先计算该父节点有多少个子节点
            int secCount = 0;
            for (int j = 0; j < count; j++)
            {
                QString secItem = listItem.at(j);
                QStringList secList = secItem.split("|");

                if (secList.count() < 4)
                {
                    continue;
                }

                QString secFatherTitle  = secList.at(1);
                if (secFatherTitle == title)
                {
                    secCount++;
                }
            }
            // 查找该父节点是否有对应子节点,有则加载
            int currentCount = 0;
            for (int j = 0; j < count; j++)
            {
                QString secItem = listItem.at(j);
                QStringList secList = secItem.split("|");

                if (secList.count() < 4)
                {
                    continue;
                }
                QString secTitle        = secList.at(0);
                QString secFatherTitle  = secList.at(1);
                QString secInfo         = secList.at(3);
                QString secIconName     = secList.at(4);

                if (secFatherTitle == title)
                {
                    currentCount++;
                    TreeNode *secNode = new TreeNode;
                    secNode->label    = secTitle;
                    secNode->info     = secInfo;
                    secNode->collapse = false;
                    secNode->level    = 2;
                    secNode->theFirst = (currentCount == 1);
                    secNode->theLast  = (currentCount == secCount);
                    secNode->iconName = secIconName;
                    node->children.push_back(secNode);
                }
            }
            treeNode.push_back(node);
        }
    }
    refreshList();
    beginResetModel();
    endResetModel();
}

int NavModel::rowCount(const QModelIndex &parent) const
{
    return listNode.size();
}

QVariant NavModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= listNode.size() || index.row() < 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return listNode[index.row()].label;
    } else if (role == Qt::UserRole) {
        return reinterpret_cast<quint64>(listNode[index.row()].treeNode);
    }
    return QVariant();
}

void NavModel::refreshList()
{
    listNode.clear();

    for (std::vector<TreeNode *>::iterator it = treeNode.begin(); it != treeNode.end(); ++it) {
        ListNode node;
        node.label = (*it)->label;
        node.treeNode = *it;

        listNode.push_back(node);

        if ((*it)->collapse) {
            continue;
        }

        for (std::list<TreeNode *>::iterator child = (*it)->children.begin(); child != (*it)->children.end(); ++child) {
            ListNode node;
            node.label = (*child)->label;
            node.treeNode = *child;
            node.treeNode->theLast = false;
            listNode.push_back(node);
        }

        if (!listNode.empty()) {
            listNode.back().treeNode->theLast = true;
        }
    }
}

void NavModel::collapse(const QModelIndex &index)
{
    TreeNode *node = listNode[index.row()].treeNode;

    if (node->children.size() == 0) {
        return;
    }

    node->collapse = !node->collapse;

    if (!node->collapse) {
        beginInsertRows(QModelIndex(), index.row() + 1, index.row() + node->children.size());
        endInsertRows();
    } else {
        beginRemoveRows(QModelIndex(), index.row() + 1, index.row() + node->children.size());
        endRemoveRows();
    }

    // 刷新放在删除行之后，放在删除行之前可能导致rowCount返回数据错误
    refreshList();
}

NavListView::NavListView(QWidget *parent) : QListView(parent)
{
    infoVisible = true;
    lineVisible = true;
    icoColorBg = false;
    style = NavListView::IcoStyle_Cross;
    colorLine         = QColor(214, 216, 224);
    colorBgNormal     = QColor(239, 241, 250);
    colorBgSelected   = QColor(133, 153, 216);
    colorBgHover      = QColor(209, 216, 240);
    colorTextNormal   = QColor(58, 58, 58);
    colorTextSelected = QColor(255, 255, 255);
    colorTextHover    = QColor(59, 59, 59);

    this->setMouseTracking(true);
    model = new NavModel(this);
    delegate = new NavDelegate(this);
    connect(this, SIGNAL(clicked(QModelIndex)), model, SLOT(collapse(QModelIndex)));
}

NavListView::~NavListView()
{
    delete model;
    delete delegate;
}

void NavListView::readData(QString xmlPath)
{
    model->readData(xmlPath);
    this->setModel(model);
    this->setItemDelegate(delegate);
}

void NavListView::setData(QStringList listItem)
{
    model->setData(listItem);
    this->setModel(model);
    this->setItemDelegate(delegate);
}

void NavListView::setCurrentRow(int row)
{
    QModelIndex index = model->index(row, 0);
    setCurrentIndex(index);
}

void NavListView::setInfoVisible(bool infoVisible)
{
    this->infoVisible = infoVisible;
}

void NavListView::setLineVisible(bool lineVisible)
{
    this->lineVisible = lineVisible;
}

void NavListView::setIcoColorBg(bool icoColorBg)
{
    this->icoColorBg = icoColorBg;
}

void NavListView::setIcoStyle(NavListView::IcoStyle style)
{
    this->style = style;
}

void NavListView::setColorLine(QColor colorLine)
{
    this->colorLine = colorLine;
}

void NavListView::setColorBg(QColor colorBgNormal,  ///< 正常背景颜色
                             QColor colorBgSelected,///< 选中背景颜色
                             QColor colorBgHover)   ///< 鼠标悬停背景颜色
{
    this->colorBgNormal   = colorBgNormal;
    this->colorBgSelected = colorBgSelected;
    this->colorBgHover    = colorBgHover;
}
void NavListView::setColorText(QColor colorTextNormal,  ///< 正常字体颜色
                               QColor colorTextSelected,///< 选中字体颜色
                               QColor colorTextHover)   ///< 鼠标悬停字体颜色
{
    this->colorTextNormal   = colorTextNormal;
    this->colorTextSelected = colorTextSelected;
    this->colorTextHover    = colorTextHover;
}
void NavListView::setColorBgLeval2(QColor _colorBgNormalLeval2,
                                   QColor _colorBgSelectedLeval2,
                                   QColor _colorBgHoverLeval2)
{
    this->colorBgNormalLeval2   = _colorBgNormalLeval2;
    this->colorBgSelectedLeval2 = _colorBgSelectedLeval2;
    this->colorBgHoverLeval2    = _colorBgHoverLeval2;
}
void NavListView::setColorTextLeval2(QColor _colorTextNormalLeval2,
                                     QColor _colorTextSelectedLeval2,
                                     QColor _colorTextHoverLeval2)
{
    this->colorTextNormalLeval2   = _colorTextNormalLeval2;
    this->colorTextSelectedLeval2 = _colorTextSelectedLeval2;
    this->colorTextHoverLeval2    = _colorTextHoverLeval2;
}
