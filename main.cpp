#include <QtWidgets/QApplication>
#include "DMDController.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DMDController w;
    w.show();
    return a.exec();
}
