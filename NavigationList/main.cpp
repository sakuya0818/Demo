#include "NavigationList.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NavigationList w;
    w.show();
    return a.exec();
}
