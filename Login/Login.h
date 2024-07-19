#ifndef LOGIN_H
#define LOGIN_H

#include "LoginTitleBar.h"

#include <QDialog>
#include <QLineEdit>
#include <QMovie>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

//重写lineedit类
class LoginLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LoginLineEdit(QWidget* parent = NULL)
        : QLineEdit(parent)
    {


    }

    void setPlaceholderText(const QString &text)
    {
        m_placeHolderText = text;
        QLineEdit::setPlaceholderText(text);
    }

private:

    void focusInEvent(QFocusEvent *event)
    {
        if (this->text().isEmpty())
        {
            QLineEdit::setPlaceholderText("");
        }

        emit signalIsMouseIn(true);
        return QLineEdit::focusInEvent(event);
    }

    void focusOutEvent(QFocusEvent *event)
    {
        if (this->text().isEmpty())
        {
            this->setPlaceholderText(m_placeHolderText);
        }

        emit signalIsMouseIn(false);
        return QLineEdit::focusOutEvent(event);
    }

signals:
    // 鼠标是否点击进行编辑;
    void signalIsMouseIn(bool isFocusIn);

private:
    QString m_placeHolderText;
};

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private:
    void initWidget();                      //初始化界面

    void initTitleBar();                    //初始化窗口标题栏

    void paintEvent(QPaintEvent *event);

private slots:
    void onButtonMinClicked();

    void onButtonCloseClicked();

private:
    Ui::Login *ui;

    LoginTitleBar *m_titleBar;
};
#endif // LOGIN_H
