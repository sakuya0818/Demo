#include "Login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    // 设置窗口背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);
    // FramelessWindowHint属性设置窗口去除边框;
    // WindowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口可以显示出原窗口;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    // 注意！！！如果是主窗口不要设置WA_DeleteOnClose属性;
    // 关闭窗口时释放资源;
    setAttribute(Qt::WA_DeleteOnClose);

    // 初始化标题栏和界面
    initWidget();
    initTitleBar();

    //设置阴影边框;
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setBlurRadius(10);
    this->setGraphicsEffect(shadowEffect);
}

Login::~Login()
{
    delete ui;
}

void Login::initWidget()
{
    // 设置两个输入框前后的图标及按钮;
    QLabel* m_labelUser = new QLabel;
    m_labelUser->setFixedSize(QSize(20, 24));
    m_labelUser->setPixmap(QIcon(":/Image/user.png").pixmap(m_labelUser->size()));

    QLabel* m_labelPassWord = new QLabel;
    m_labelPassWord->setFixedSize(QSize(26, 26));
    m_labelPassWord->setPixmap(QIcon(":/Image/password.png").pixmap(m_labelPassWord->size()));

    QPushButton* m_pButtonPullDown = new QPushButton;
    m_pButtonPullDown->setCursor(Qt::ArrowCursor);
    m_pButtonPullDown->setFixedSize(QSize(24, 24));
    m_pButtonPullDown->setStyleSheet("QPushButton{border-image:url(:/Image/down.png);}\
                                        QPushButton:hover{border-image:url(:/Image/down_hover.png);}");

    QPushButton* m_pButtonKeyborad = new QPushButton;
    m_pButtonKeyborad->setCursor(Qt::PointingHandCursor);
    m_pButtonKeyborad->setFixedSize(QSize(24, 24));
    m_pButtonKeyborad->setStyleSheet("QPushButton{border-image:url(:/Image/jianpan.png);}\
                                        QPushButton:hover{border-image:url(:/Image/jianpan_hover.png);}\
                                        QPushButton:pressed{border-image:url(:/Image/jianpan.png) 0 0 0 80;}");

    ui->Btn_login->setStyleSheet("QPushButton{border-image:url(:/Image/login.png);}\
                                        QPushButton:hover{border-image:url(:/Image/login_hover.png);}\
                                        QPushButton:pressed{border-image:url(:/Image/login_pressed.png);}");

    ui->lineEdit_Account->setPlaceholderText(QString::fromUtf8("请输入用户名"));
    ui->lineEdit_Account->setTextMargins(25, 0, 25, 0);

    // 这里通过signalIsMouseIn信号进行图标切换；
    connect(ui->lineEdit_Account, &LoginLineEdit::signalIsMouseIn, this, [=](bool isMouseIn) {
        if (isMouseIn)
        {
            m_labelUser->setPixmap(QIcon(":/Image/user_hover.png").pixmap(m_labelUser->size()));
        }
        else
        {
            m_labelUser->setPixmap(QIcon(":/Image/user.png").pixmap(m_labelUser->size()));
        }
    });

    QHBoxLayout* hAccountLayout = new QHBoxLayout(ui->lineEdit_Account);
    hAccountLayout->addWidget(m_labelUser);
    hAccountLayout->addStretch();
    hAccountLayout->addWidget(m_pButtonPullDown);
    hAccountLayout->setMargin(0);

    ui->lineEdit_PassWord->setPlaceholderText("密码");
    ui->lineEdit_PassWord->setTextMargins(25, 0, 25, 0);
    connect(ui->lineEdit_PassWord, &LoginLineEdit::signalIsMouseIn, this, [=](bool isMouseIn) {
        if (isMouseIn)
        {
            m_labelPassWord->setPixmap(QIcon(":/Image/password_hover.png").pixmap(m_labelPassWord->size()));
        }
        else
        {
            m_labelPassWord->setPixmap(QIcon(":/Image/password.png").pixmap(m_labelPassWord->size()));
        }
    });

    this->setStyleSheet("*{font-family:Microsoft YaHei;font-size:12px;}\
                            QLineEdit{font-size:20px;background:transparent;border:none;border-bottom:1px solid rgb(229, 229, 229);}\
                            QLineEdit:hover{border-bottom:1px solid rgb(193,193, 193);}\
                            QLineEdit:focus{border-bottom:1px solid rgb(18, 183, 245);}");

    QHBoxLayout* hPswLayout = new QHBoxLayout(ui->lineEdit_PassWord);
    hPswLayout->addWidget(m_labelPassWord);
    hPswLayout->addStretch();
    hPswLayout->addWidget(m_pButtonKeyborad);
    hPswLayout->setMargin(0);

    //背景GIF图;
    QMovie *movie = new QMovie();
    movie->setFileName(":/Image/GIF.gif");
    ui->label->setMovie(movie);
    movie->start();

    QLabel *logo = new QLabel(this);
    logo->setGeometry(QRect(0, 0, 540, 200));
    logo->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    logo->setText("Title");
    logo->setStyleSheet("background:transparent;font-size:36px;font:bold;color:white;");
    logo->move(0, 0);
}

void Login::initTitleBar()
{
    m_titleBar = new LoginTitleBar(this);
    m_titleBar->move(6, 6);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setWindowBorderWidth(12);
}

void Login::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(6, 6, this->width() - 12, this->height() - 12), 6, 6);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.fillPath(pathBack, QBrush(QColor(225, 240, 245)));

    return QWidget::paintEvent(event);
}

void Login::onButtonMinClicked()
{
    showMinimized();
}

void Login::onButtonCloseClicked()
{
    close();
}
