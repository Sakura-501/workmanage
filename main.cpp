#include "mainwindow.h"

#include <QApplication>
#include<dialog.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dialog d;
    d.show();
    //MainWindow w;
   // w.show();
    return a.exec();
}
