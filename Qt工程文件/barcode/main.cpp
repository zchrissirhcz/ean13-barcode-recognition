#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
/*
 *在qt中的main（）函数是编译器自动建立的，这里没有做修改，也没有必要修改*/
