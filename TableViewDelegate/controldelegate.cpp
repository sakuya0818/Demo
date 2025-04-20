#include "controldelegate.h"

//spinbox控件部分
SpinboxDelegate::SpinboxDelegate(int column)
{
        mColumn = column;
}

//返回修改数据的组件，为指定的列或者行创建部件
QWidget *SpinboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    if(index.column() == 3)
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setRange(0 , 1000);
        connect(editor,SIGNAL(editingFinished()),SLOT(commitAndCloseEditor()));
        return editor;
    }
    else if( index.column() == 4)
    {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setRange(0,1000);
        connect(editor,SIGNAL(editingFinished()),SLOT(commitAndCloseEditor()));
        return editor;
    }
    else
    {
        return QItemDelegate::createEditor(parent,option,index);
    }
}

//根据editor 的数据更新model的数据
void SpinboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

//显示格式控制
void SpinboxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 3) {
        int warehouseAmount = index.model()->data(index,Qt::DisplayRole).toInt();
        QString text = QString("%1").arg(warehouseAmount, 3, 10, QChar(' '));

        QStyleOptionViewItem myOption = option;
        //设置显示在item的中间
        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    }
    else if(index.column() == 4) {
        int amount = index.model()->data(index,Qt::DisplayRole).toInt();
        QString text = QString("%1").arg(amount, 3, 10, QChar(' '));

        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    }
}

void SpinboxDelegate::commitAndCloseEditor()
{
    QSpinBox *editor = qobject_cast<QSpinBox*>(sender());
    emit commitData(editor);                    //当编辑器小部件完成数据编辑并希望将其写入模型时，必须发出此信号
    emit closeEditor(editor);                   //当用户使用指定的编辑器完成对项目的编辑时，将发出此信号
}

//checkbox部分
static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)/*const*/
{
    //绘制按钮所需要的参数
    QStyleOptionButton checkBoxStyleOption;
    //按照给定的风格参数 返回元素子区域
    QRect checkBoxRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    //返回QCheckBox坐标
    QPoint checkBoxPoint(viewItemStyleOptions.rect.x() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2,
                         viewItemStyleOptions.rect.y() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2);
    //返回QCheckBox几何形状
    return QRect(checkBoxPoint, checkBoxRect.size());
}

CheckBoxDelegate::CheckBoxDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void CheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    if(index.column() == 5) {
        //按钮的风格选项
        QStyleOptionButton checkBoxStyleOption;
        //|=为复合赋值语句，a|=b等价于a=a|b，对a和b进行位或运算
        checkBoxStyleOption.state |= QStyle::State_Enabled;

        //根据值判断是否选中
        checkBoxStyleOption.state |= checked? QStyle::State_On : QStyle::State_Off;

        //返回QCheckBox几何形状
        checkBoxStyleOption.rect = CheckBoxRect(option);
        //绘制QCheckBox
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyleOption, painter);
    }
    else {
        //否则调用默认委托
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool CheckBoxDelegate::editorEvent(QEvent *event,
                                QAbstractItemModel *model,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) {
    if(index.column() == 5){
    if((event->type() == QEvent::MouseButtonRelease) ||
            (event->type() == QEvent::MouseButtonDblClick)){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() != Qt::LeftButton ||
                !CheckBoxRect(option).contains(mouseEvent->pos())){
            return true;
        }
        if(event->type() == QEvent::MouseButtonDblClick){
            return true;
        }
    }else if(event->type() == QEvent::KeyPress){
        if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select){
            return false;
        }
    }else{
        return false;
    }

    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
    return model->setData(index, !checked, Qt::EditRole);
    }else{
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}

//combobox控件部分
ComboboxDelegate::ComboboxDelegate()
{

}

QWidget *ComboboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    if(index.column() == 2){
        editor->addItem(QString::fromLocal8Bit("boy"));
        editor->addItem(QString::fromLocal8Bit("girl"));
        editor->setCurrentIndex(0);

        connect(editor,SIGNAL(editingFinished()),SLOT(commitAndCloseEditor()));
        return editor;
    }
    else
    {
        QItemDelegate::createEditor(parent,option,index);
    }
}

void ComboboxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2)
    {
        QString text = index.model()->data(index , Qt::DisplayRole).toString();

        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

        drawDisplay(painter,myOption,myOption.rect,text);
        drawFocus(painter,myOption,myOption.rect);
    }
    else
    {
        QItemDelegate::paint(painter,option,index);
    }
}

void ComboboxDelegate::commitAndCloseEditor()
{
    QComboBox *editor = qobject_cast<QComboBox*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
