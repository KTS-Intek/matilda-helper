#include <QCoreApplication>
#include "consoleclass.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ConsoleClass cc;
    return cc.checkArgs(a.arguments());

}
