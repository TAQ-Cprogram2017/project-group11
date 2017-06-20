#include "mainwindow.h"
#include "soundsplayer.h"
#include <QApplication>    //QApplication类管理GUI应用程序控制流程和主要设置。每一个使用Qt图形化应用程序中都必须使用
//#include <QPixmap>  //图像
//#include <QElapsedTimer>  //计时器
//使用哪个类就必须将包含该类的头文件引用过来。 按钮类#include<QPushButton>

int main(int argc, char *argv[])//应用程序的入口，几乎在所有使用Qt的情况下，main（）函数只需要在将控制转交给Qt库之前执行初始化。然后Qt库通过事件向程序告知用户的行为。Qt只有一个main函数，两个参数，argc是命令行变量的数量，argv是命令变行量数组
{
    QApplication a(argc, argv);//a是这个程序QApplication对象



    MainWindow w;
    w.show();//创建一个窗口部件的时候，默认其实不可见的，必须调用shou（）使其可见


    return a.exec();//程序进入消息循环，等待可能的输入进行响应。当应用程序退出的时候，exec（）函数的值就会返回。在exec函数中，Qt接收并处理用户和系统的事件并且将它们传递给适当的窗口部件。
}
