#ifndef DETECT_H
#define DETECT_H

/*version 0.02
*提高了识别精度
*第一位字符通过左侧6个数据反推得到，不仅仅限定于6和9
*------
*     ChrisZZ  2012.5.5
*/
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define SPACE 0         //定义“空”为0
#define BAR 255            /*定义“条”为255   正常情况下白色是255，但是在处理图片的时候把白色和黑色做了对调的处理（先二值化为灰度图像然后对调颜色，使用了bitwize_not() 函数，这个是opencv函数*/

typedef Mat_<uchar> MatU;
enum position{LEFT,RIGHT};
int front;       //起始编号
int mode[6];    //不同的起始编号对应着不同的左侧编码模式，比如起始编号为6对应“奇偶偶偶奇奇”，9对应“奇偶偶奇偶奇”。用1表示奇数，0表示偶数
const double eps=1e-5;//定义eps为0.00001，用于浮点数的比较。这里面是at1和at2的比较那里用到
int z;

//----zz-----

string RES="1234567890123";                 /*RES存储最终识别的结果。初始化时候设定为1234567890123，只是为了保证是13位*/
//----zz-----


int get_front(){/*获取条码最前面的一位(d0).是通过条码中d1-d6反推得到的 */
    /*mode[]数组是在识别d1-d6的时候就存储好的，这里把mode[]转换到string类型的tmp里面只是为了映射到相应的d0的值时候方便判断*/
    /*如果mode[i]=1,那么di就是奇数形式的（也就是表示成7个二进制数字的时候1的个数是奇数个），如果mode[i]=0表示对应偶数个1*/
    string tmp="";
    int i;
    for(i=0; i<6; i++){
        if(mode[i]==0) tmp=tmp+"0";
        else tmp=tmp+"1";
    }
    /*由于d0可能的取值为0-9，反映了d1-d6的奇偶性，是提前规定好的，这里只是做一个简单的映射*/
    if(tmp=="000000") return 0;
    else if(tmp=="001011") return 1;
    else if(tmp=="001101") return 2;
    else if(tmp=="001110") return 3;
    else if(tmp=="010011") return 4;
    else if(tmp=="011001") return 5;
    else if(tmp=="011100") return 6;
    else if(tmp=="010101") return 7;
    else if(tmp=="010110") return 8;
    else if(tmp=="011010") return 9;
    /*但是tmp存储的6个数字可能因为测量误差不准确，不能对应到0-9，那么说明识别失败，那么返回-1*/
    else return -1;
}

/*int read_digit(...)
  *返回解析后得到的数字*/
int read_digit(const MatU& img, Point& cur, int position){/*img是读入的黑白图像，cur表示当前点的坐标，position表示在判断的数字是左侧(d1-d6)还是(d7-d13)*/
    int pattern[4] = {0,0,0,0}, i;/*pattern[]数组表示条码中的一个数字对应的“黑白黑白”或“白黑白黑”对应的宽度分别是多少*/
    for (i=0; i<4; i++){
        int cur_val=img(cur);/*标记值，记录当前颜色*/
        while(img(cur)==cur_val){/*只要当前颜色和标记值一样说明没有颜色变化，那么就让扫描坐标向右移动*/
            ++pattern[i];
            ++cur.x;
        }
    }
    /*使用归一化方法测量条码值*/
    /*假设条码中的某个数字对应的条码区域的宽度分别是m1,m2,m3,m4*/
    /*tmp1表示m1+m2的宽度，tmp2表示m2+m3的宽度， sum表示m1+m2+m3+m4也就是总宽度*/
    /*通过tmp1和sum的比较以及tmp2和sum的比较能确定m1,m2,m3,m4的真实值是多少，因为测量的时候的m1,m2,m3,m4太小了可能有误差*/
    double sum=pattern[0]+pattern[1]+pattern[2]+pattern[3];
    double tmp1=(pattern[0]+pattern[1])*1.0;
    double tmp2=(pattern[1]+pattern[2])*1.0;
    int at1, at2;
    if(tmp1/sum < 2.5/7)  at1=2;
    else if(tmp1/sum < 3.5/7) at1=3;
    else if(tmp1/sum < 4.5/7) at1=4;
    else at1=5;

    if(tmp2/sum < 2.5/7)  at2=2;
    else if(tmp2/sum < 3.5/7) at2=3;
    else if(tmp2/sum < 4.5/7) at2=4;
    else at2=5;

    int digit=-1;/*digit用来存储最终识别的结果的值，初始设定为-1，如果下面没有匹配到的话，digit的值不会变，-1就表示识别失败*/

    if(position==LEFT){/*如果是左侧d1-d6的区域*/
        /*
         *这里at1表示按照比例调整后，m1+m2的宽度，at2表示m2+m3的宽度，前面说的tmp1和tmp2是测量值
         *然后按照提前计算好的映射表格去做映射就可以了。这个映射可以看一下ppt的
         *当然这里也有mode[]数组的参与，是判断d1-d6的每个数字的奇偶性*
        */
        if(at1==2){
            if(at2==2) {
                mode[z++]=0;
                digit = 6;
            }
            else if(at2==3) {
                mode[z++]=1;
                digit = 0;
            }
            else if(at2==4) {
                mode[z++]=0;
                digit = 4;
            }
            else if(at2==5) {
                mode[z++]=1;
                digit = 3;
            }
        }
        else if(at1==3){/*如果是右侧d7-d13的区域*/
            if(at2==2) {
                mode[z++]=1;
                digit = 9;
            }
            else if(at2==3) {
                mode[z++]=0;
                if(pattern[2]+1<pattern[3]) digit = 8;
                else digit = 2;
            }
            else if(at2==4) {
                mode[z++]=1;
                if(pattern[1]+1<pattern[2]) digit = 7;
                else digit = 1;
            }
            else if(at2==5) {
                mode[z++]=0;
                digit = 5;
            }
        }
        else if(at1==4){
            if(at2==2) {
                mode[z++]=0;
                digit = 9;
            }
            else if(at2==3) {
                mode[z++]=1;
                if(pattern[1]+1<pattern[0]) digit = 8;
                else digit = 2;
            }
            else if(at2==4) {
                mode[z++]=0;
                if(pattern[0]+1<pattern[1]) digit = 7;
                else digit = 1;
            }
            else if(at2==5) {
                mode[z++]=1;
                digit = 5;
            }
        }
        else if(at1==5){
            if(at2==2) {
                mode[z++]=1;
                digit = 6;
            }
            else if(at2==3) {
                mode[z++]=0;
                digit = 0;
            }
            else if(at2==4) {
                mode[z++]=1;
                digit = 4;
            }
            else if(at2==5) {
                mode[z++]=0;
                digit=3;
            }
        }

    }else{
        /*
         *这里是右侧区域的d7-d12的映射，也是利用提前计算好的表格做映射。
        */
        if(at1==2){
            if(at2==2) digit = 6;
            else if(at2==4) digit = 4;
        }
        else if(at1==3){
            if(at2==3) {
                if(pattern[2]+1<pattern[3]) digit = 8;
                else digit = 2;
            }
            else if(at2==5) digit = 5;
        }
        else if(at1==4){
            if(at2==2) digit = 9;
            else if(at2==4) {
                if(pattern[0]+1<pattern[1]) digit = 7;
                else digit = 1;
            }
        }
        else if(at1==5){
            if(at2==3) digit = 0;
            else if(at2==5) digit=3;
        }
    }
    return digit;
}

void skip_quiet_zone(const MatU& img, Point& cur){//略过空白区域
    while (img(cur) == SPACE)/*只要是SPACE的区域就略过直到遇到BAR的区域*/
        ++cur.x;
}

void read_lguard(const MatU& img, Point& cur){//读取左侧固定的“条空条”,目的是调整扫描点cur的位置
    int pattern[3] = { BAR, SPACE, BAR };/*对应于“101”模式，pattern[]数组目的在于统计扫描过的条码变化次数。这里是左侧的区域那么是3次，如果是后面的m_gurad区域的话对应的就是5次扫描了*/
    for (int i=0; i<3; i++)
        while (img(cur)==pattern[i])
            ++cur.x;
}

void skip_mguard(const MatU& img, Point& cur){      //略过左侧数据和右侧数据之间的分界
    int pattern[5] = { SPACE, BAR, SPACE, BAR, SPACE };
    for (int i=0; i<5; i++)
        while (img(cur)==pattern[i])
            ++cur.x;
}

void read_barcode(const string& filename){      //读取条码主程序
    z=0;
    MatU img = imread(filename, 0);         //载入灰度图像
    Size size = img.size();
    Point cur(0, size.height / 2);              //cur表示当前位置，这里取(0,h/2）的位置，h表示图像的高度
    bitwise_not(img, img);      //黑白对调。之所以这么做是为了提高测量精确度
    threshold(img, img, 128, 255, THRESH_BINARY);       /*对灰度图像进行阈值操作得到二值图像*/
    skip_quiet_zone(img, cur);      //略过左侧的“静区”，也就是人眼看到的白色区域
    read_lguard(img, cur);      //读取左侧的6个数字
    vector<int> digits;     //digit向量用于存储识别的结果
    for (int i=0; i<6; i++){               //左侧数据解码
        int d = read_digit(img, cur, LEFT);
        digits.push_back(d);        //将解码的结果存储到向量digit中
    }
    skip_mguard(img, cur);//略过中间的“白黑白黑白”区域
    for (int i = 0; i < 6; i++){        //右侧数据解码
        int d = read_digit(img, cur, RIGHT);
        digits.push_back(d);        //将解码的结果存储到向量digit中
    }
    //整理解码结果
    RES[0]=get_front()+'0';//RES[0]对应于d0，由于RES是string型数据，get_front()返回的是d0的数值，需要加上'0'才能表示成字符中的对应的数字
    for (int i = 0; i < 12; i++){
        RES[i+1]=digits[i]+'0';/*将digit中的数据存储到RES中。digit也是整数类型所以也要加上'0'才能表示对应的字符*/
    }
}


#endif // DETECT_H
