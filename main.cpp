#include "mainwindow.h"
#include "player.h"

#include <QApplication>
#include <unistd.h>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    //w.update();
    w.show();
    return a.exec();
}
