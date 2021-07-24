/********************************************************************************
* File Name:	mainwindow.cpp
* Description:	第3章 实例9：Group Box和ScrollArea的实例
* Reference book:《Linux环境下Qt4图形界面与MySQL编程》，机械工业出版社.2012.1
* E_mail: openlinux2011@gmail.com
*
********************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

#include <QString>
#include <vector>
#include <iostream>
#include <string>
#include "barcode_recog.h"

using namespace std;
using namespace cv;

extern string RES;
/*RES是条码识别结果，由于在detect.h文件中和mainwindow.cpp中都有使用所以用extern声明*/

/*Qt中有QString，但是detect.h中的识别过程使用的是string,需要做两者的互相转化*/
QString s2q(const string &s);/*------------------------*/
string q2s(const QString &s);

QString s2q(const string &s)/*----------------string到QString的函数---------------------- */
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}
string q2s(const QString &s)/*-----------------------QString到string的函数 ------------------*/
{
    return string((const char *)s.toLocal8Bit());
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = new QLabel;
    ui->scrollArea->setWidget(label);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()/*-------------------"打开"按钮，当被按下的时候进行处理----------------------*/
{
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty())/*若文件存在那么就打开*/
    {
        QImage image(fileName);
        if (image.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        label->setPixmap(QPixmap::fromImage(image));
    }
}


void MainWindow::on_pushButton_clicked()/*-----------------"开始识别"按钮，按下之后开始识别-------------------*/
{
    string imgname=q2s(fileName);/*imgname为图像的包含路径的名字,利用q2s转化为string类型得到*/
    read_barcode(imgname);/*调用核心的检测代码检测*/


    QString res= s2q(RES);/*把检测结果RES转换为QString类型*/
    ui->label2->setText(res);//将转换后的结果显示在label2中
}


void MainWindow::on_pushButton_2_clicked()/*------------------关闭程序-----------*/
{
   QApplication::quit();/*当按下“关闭”按钮的时候关闭程序*/
}
