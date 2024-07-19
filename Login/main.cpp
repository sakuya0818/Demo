#include "Login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login *w = new Login();
    w->show();

    return a.exec();
}
