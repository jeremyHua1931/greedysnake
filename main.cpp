#include "gamewidget.h"
#include "mainwindow.h"
#include <QApplication>

/**
 * @brief 主函数，程序入口
 *
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return int 程序退出状态码
 */
int main(int argc, char *argv[])
{
    // 创建Qt应用程序对象
    QApplication a(argc, argv);
    // 创建主窗口对象
    MainWindow w;
    // 显示主窗口
    w.show();
    // 进入Qt事件循环，等待事件处理
    return a.exec();
}
