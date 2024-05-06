#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QtDebug>
#include <windows.h>
#include <QThread>
#include <QStyle>
#include <QtGlobal>
#include <QTime>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QRegExp>

GameWidget::GameWidget(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    initGame();                                                                // 初始化游戏
    initBorder();                                                              // 初始化边界
    initSnake();                                                               // 初始化蛇
    canCreat = true;                                                           // 允许创建
    memset(clicked, true, sizeof(clicked));                                    // 初始化clicked数组
    pressed = false;                                                           // 按钮未按下
    canMove = false;                                                           // 不能移动
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(snakeMoveSlots())); // 连接计时器信号与蛇移动槽函数
}

void GameWidget::initGame()
{
    steps = 0; // 步数初始化为0
    for (int x = 0; x < MAX_X; x++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            QLabel *label = new QLabel(this);                                                     // 创建新的QLabel控件
            label->setGeometry(x * Label_Size + 10, y * Label_Size + 10, Label_Size, Label_Size); // 设置控件的位置和大小
            label->setStyleSheet("background:gray");                                              // 设置控件的背景颜色为灰色
            label->hide();                                                                        // 隐藏控件
            map_label[x][y] = new Snake;                                                          // 创建新的Snake结构体实例并赋值给map_label数组
            map_label[x][y]->x = x;                                                               // 设置Snake结构体的x坐标
            map_label[x][y]->y = y;                                                               // 设置Snake结构体的y坐标
            map_label[x][y]->type = bg_label;                                                     // 设置Snake结构体的类型为背景标签
            map_label[x][y]->label = label;                                                       // 将QLabel控件赋值给Snake结构体中的label成员
        }
    }
}

void GameWidget::initBorder()
{
    for (int y = 0; y < MAX_Y; y++)
    {
        map_label[0][y]->type = border_label;                              // 设置边界标签类型为边界标签
        map_label[0][y]->label->setStyleSheet("background:black");         // 设置背景颜色为黑色
        map_label[0][y]->label->show();                                    // 显示边界标签
        map_label[MAX_X - 1][y]->type = border_label;                      // 设置边界标签类型为边界标签
        map_label[MAX_X - 1][y]->label->setStyleSheet("background:black"); // 设置背景颜色为黑色
        map_label[MAX_X - 1][y]->label->show();                            // 显示边界标签
    }
    for (int x = 0; x < MAX_X; x++)
    {
        map_label[x][0]->type = border_label;                              // 设置边界标签类型为边界标签
        map_label[x][0]->label->setStyleSheet("background:black");         // 设置背景颜色为黑色
        map_label[x][0]->label->show();                                    // 显示边界标签
        map_label[x][MAX_Y - 1]->type = border_label;                      // 设置边界标签类型为边界标签
        map_label[x][MAX_Y - 1]->label->setStyleSheet("background:black"); // 设置背景颜色为黑色
        map_label[x][MAX_Y - 1]->label->show();                            // 显示边界标签
    }
}

void GameWidget::initSnake()
{
    int initX = 10, initY = 10;
    int snakelen = 2;
    dX = 1;
    dY = 0;
    snake.clear(); // 清空蛇的信息
    for (int i = 0; i < snakelen; i++)
    {
        map_label[initX + i][initY]->type = snake_label;                                                         // 设置蛇身位置的类型为snake_label
        map_label[initX + i][initY]->label->setStyleSheet("background:green;border:1px solid rgb(240,240,240)"); // 设置蛇身位置的样式
        map_label[initX + i][initY]->label->show();                                                              // 显示蛇身位置的标签
        snake.append(map_label[initX + i][initY]);                                                               // 将蛇身位置添加到蛇的数组中
    }
    snake[snake.length() - 1]->label->setStyleSheet("background:green;border-radius:" + QString::number(Label_Size / 2)); // 设置蛇头部分的样式
}

void GameWidget::moveSnake()
{
    steps++;                                                                                        // 步数加1
    emit displayStepSignal(steps);                                                                  // 发送步数信号
    tail = snake.at(0);                                                                             // 获取蛇尾
    head = snake.at(snake.length() - 1);                                                            // 获取蛇头
    Snake *tmp = map_label[head->x + dX][head->y + dY];                                             // 获取蛇头下一步位置
    head->label->setStyleSheet("background:green;border:1px solid rgb(240,240,240)");               // 设置蛇头样式
    tmp->label->setStyleSheet("background:green;border-radius:" + QString::number(Label_Size / 2)); // 设置蛇身样式
    tmp->label->show();                                                                             // 显示蛇身
    if (tmp->type == border_label || tmp->type == snake_label)                                      // 如果下一步是边界或者蛇身
    {
        gameOver(); // 游戏结束
    }
    else
    {
        if (tmp->type == food_label) // 如果下一步是食物
        {
            scores++;                        // 分数加1
            emit displayScoreSignal(scores); // 发送分数信号
            tmp->type = snake_label;         // 设置下一步为蛇身
            for (int i = 0; i < 1; i++)      // 蛇身增加1节
                snake.append(tmp);
            createFood(); // 创建新的食物
        }
        else
        {
            tmp->type = snake_label; // 设置下一步为蛇身
            snake.append(tmp);       // 将蛇身加入蛇的列表
            tail->type = bg_label;   // 尾部变为背景
            tail->label->hide();     // 隐藏尾部
            snake.removeFirst();     // 移除蛇的头部
        }
    }
}

void GameWidget::createFood()
{
    int foodX = 0;
    int foodY = 0;
    srand((unsigned)time(0)); // 设置随机种子
    do
    {
        foodX = rand() % MAX_X;                                      // 随机生成食物的 X 坐标
        foodY = rand() % MAX_Y;                                      // 随机生成食物的 Y 坐标
    } while (map_label[foodX][foodY]->type != bg_label);             // 直到食物的位置是背景
    map_label[foodX][foodY]->type = food_label;                      // 设置食物类型为 food_label
    map_label[foodX][foodY]->label->setStyleSheet("background:red"); // 设置食物样式为红色
    map_label[foodX][foodY]->label->show();                          // 显示食物
}

void GameWidget::keyPressEvent(QKeyEvent *e) {
   if (canMove){
    switch(e->key()) {
    case Qt::Key_Left:
        // 当蛇不是向右移动时才能向左移动
        if (dX != 1) {
            dX = -1;
            dY = 0;
            if (!pressed) {
                timer.start(speed);
                pressed = true;
            }
        }
        break;
    case Qt::Key_Right:
        // 当蛇不是向左移动时才能向右移动
        if (dX != -1) {
            dX = 1;
            dY = 0;
            if (!pressed) {
                timer.start(speed);
                pressed = true;
            }
        }
        break;
    case Qt::Key_Up:
        // 当蛇不是向下移动时才能向上移动
        if (dY != 1) {
            dX = 0;
            dY = -1;
            if (!pressed) {
                timer.start(speed);
                pressed = true;
            }
        }
        break;
    case Qt::Key_Down:
        // 当蛇不是向上移动时才能向下移动
        if (dY != -1) {
            dX = 0;
            dY = 1;
            if (!pressed) {
                timer.start(speed);
                pressed = true;
            }
        }
        break;
    default:
        break;
    }
   }
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    QPoint pointer;
    int axisX, axisY;
    if (!canCreat) // 如果不允许创建，则返回
        return;
    else
    {
        if (e->button() == Qt::LeftButton) // 检查是否为左键点击
        {
            pointer = e->localPos().toPoint(); // 获取鼠标位置
            axisX = pointer.x();
            axisY = pointer.y();
            int x = (axisX - 10) / Label_Size; // 计算格子的 x 坐标
            int y = (axisY - 10) / Label_Size; // 计算格子的 y 坐标
            if (clicked[x][y] == true)         // 如果该格子已被点击
            {
                if (map_label[x][y]->type != snake_label && x != MAX_X - 1 && x != 0 && y != MAX_Y - 1 && y != 0)
                {
                    map_label[x][y]->type = border_label;                      // 标记为边界格子
                    map_label[x][y]->label->setStyleSheet("background:black"); // 设置背景样式
                    map_label[x][y]->label->show();                            // 显示标签
                    clicked[x][y] = false;                                     // 标记为已点击
                }
            }
            else
            {
                if (map_label[x][y]->type != snake_label && x != MAX_X - 1 && x != 0 && y != MAX_Y - 1 && y != 0)
                {
                    map_label[x][y]->type = bg_label;                         // 标记为背景格子
                    map_label[x][y]->label->setStyleSheet("background:gray"); // 设置背景样式
                    map_label[x][y]->label->hide();                           // 隐藏标签
                    clicked[x][y] = true;                                     // 标记为未点击
                }
            }
        }
    }
}

void GameWidget::startGame(double movespeed)
{
    steps = 0;
    scores = 0;
    canCreat = false;                // 禁止创建边界
    canMove = true;                  // 允许移动蛇
    speed = movespeed;               // 设置移动速度
    emit displayStepSignal(steps);   // 发送当前步数信号
    emit displayScoreSignal(scores); // 发送当前得分信号
    initSnake();                     // 初始化蛇
    //    moveSnake();
    createFood(); // 创建食物
}

void GameWidget::restartGame()
{
    canCreat = true; // 允许创建边界
    steps = 0;
    scores = 0;
    emit displayStepSignal(0);  // 发送步数重置信号
    emit displayScoreSignal(0); // 发送得分重置信号
    timer.stop();               // 停止计时器
    for (int x = 0; x < MAX_X; x++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            map_label[x][y]->label->setStyleSheet("background:gray"); // 设置背景颜色为灰色
            map_label[x][y]->type = bg_label;                         // 设置类型为背景格子
            map_label[x][y]->label->hide();                           // 隐藏标签
        }
    }

    // 重新设置clicked数组
    memset(clicked, true, sizeof(clicked));

    initSnake();  // 初始化蛇
    initBorder(); // 初始化边界
}

void GameWidget::pauseGame()
{
    timer.stop(); // 暂停计时器
}

void GameWidget::continueGame(double movespeed)
{
    timer.start(movespeed); // 以指定速度继续计时器
}

void GameWidget::gameOver()
{
    QMessageBox::information(this, "Oh!No!", "Game is over! Do you want to try again?", QMessageBox::Ok); // 显示游戏结束信息框
    for (int x = 0; x < MAX_X; x++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            map_label[x][y]->label->setStyleSheet("background:gray"); // 设置标签背景颜色为灰色
            map_label[x][y]->type = bg_label;                         // 设置类型为背景格子
            map_label[x][y]->label->hide();                           // 隐藏标签
            clicked[x][y] = false;                                    // 重置点击状态
        }
    }
    canMove = false;       // 禁止移动
    pressed = false;       // 重置按钮按下状态
    initSnake();           // 初始化蛇
    initBorder();          // 初始化边界
    timer.stop();          // 停止计时器
    emit gameOverSignal(); // 发送游戏结束信号
}

void GameWidget::saveGame()
{
    timer.stop(); // 暂停计时器
    QString snakeInfo, borderInfo, foodInfo;
    for (int i = 0; i < snake.length(); i++)
    {
        snakeInfo += QString::number(snake[i]->x);
        snakeInfo += " ";
        snakeInfo += QString::number(snake[i]->y);
        snakeInfo += " ";
    }
    for (int x = 0; x < MAX_X; x++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            if (map_label[x][y]->type == border_label)
            {
                borderInfo += QString::number(x);
                borderInfo += " ";
                borderInfo += QString::number(y);
                borderInfo += " ";
            }
            else if (map_label[x][y]->type == food_label)
            {
                foodInfo += QString::number(x);
                foodInfo += " ";
                foodInfo += QString::number(y);
                foodInfo += " ";
            }
        }
    }
    QString info = snakeInfo + '\n' + borderInfo + '\n' + foodInfo + '\n' + QString::number(steps) + '\n' + QString::number(scores);
    QString path = QFileDialog::getSaveFileName(this, "save", "../", "TXT (*.snakesavedata)"); // 获取保存文件路径
    if (!path.isEmpty())
    {
        QFile file;
        file.setFileName(path);
        bool opened = file.open(QIODevice::WriteOnly);
        if (opened)
        {
            file.write(info.toUtf8().data());                                                  // 写入游戏数据
            QMessageBox::information(this, "Finished!", "Save Successfully", QMessageBox::Ok); // 显示保存成功信息框
        }
        file.close();
    }
}

void GameWidget::loadGame()
{
    QByteArray array;
    QString path = QFileDialog::getOpenFileName(this, "open", "../", "TXT (*.snakesavedata)"); // 获取打开文件路径
    if (!path.isEmpty())
    {
        QFile file(path);
        bool opened = file.open(QIODevice::ReadOnly);
        if (opened)
        {
            array = file.readLine();
            QList<QByteArray> snakelist = array.split(' ');
            array = file.readLine();
            QList<QByteArray> borderlist = array.split(' ');
            array = file.readLine();
            QList<QByteArray> foodlist = array.split(' ');
            array = file.readLine();
            steps = array.toInt();
            array = file.readLine();
            scores = array.toInt();
            for (int x = 0; x < MAX_X; x++)
            {
                for (int y = 0; y < MAX_Y; y++)
                {
                    map_label[x][y]->label->setStyleSheet("background:gray"); // 设置标签背景颜色为灰色
                    map_label[x][y]->type = bg_label;                         // 设置类型为背景格子
                    map_label[x][y]->label->hide();                           // 隐藏标签
                }
            }
            snake.clear();
            for (int i = 0; i < snakelist.length() - 1; i += 2)
            {
                if (snakelist[i] != "\n" && snakelist[i] != " ")
                {
                    map_label[snakelist[i].toInt()][snakelist[i + 1].toInt()]->type = snake_label;                                                         // 设置类型为蛇格子
                    map_label[snakelist[i].toInt()][snakelist[i + 1].toInt()]->label->setStyleSheet("background:green;border:1px solid rgb(240,240,240)"); // 设置标签样式
                    map_label[snakelist[i].toInt()][snakelist[i + 1].toInt()]->label->show();                                                              // 显示标签
                    snake.push_back(map_label[snakelist[i].toInt()][snakelist[i + 1].toInt()]);                                                            // 将格子添加到蛇身列表
                }
            }
            snake[snake.length() - 1]->label->setStyleSheet("background:green;border-radius:" + QString::number(Label_Size / 2)); // 设置蛇尾标签样式
            for (int i = 0; i < borderlist.length() - 1; i += 2)
            {
                if (borderlist[i] != "\n" && borderlist[i] != " ")
                {
                    map_label[borderlist[i].toInt()][borderlist[i + 1].toInt()]->type = border_label;                      // 设置类型为边界格子
                    map_label[borderlist[i].toInt()][borderlist[i + 1].toInt()]->label->setStyleSheet("background:black"); // 设置标签样式
                    map_label[borderlist[i].toInt()][borderlist[i + 1].toInt()]->label->show();                            // 显示标签
                }
            }
            for (int i = 0; i < foodlist.length() - 1; i += 2)
            {
                if (foodlist[i] != "\n" && foodlist[i] != " ")
                {
                    map_label[foodlist[i].toInt()][foodlist[i + 1].toInt()]->type = food_label;                      // 设置类型为食物格子
                    map_label[foodlist[i].toInt()][foodlist[i + 1].toInt()]->label->setStyleSheet("background:red"); // 设置标签样式
                    map_label[foodlist[i].toInt()][foodlist[i + 1].toInt()]->label->show();                          // 显示标签
                }
            }
            // 设置蛇移动方向
            if (snake[snake.length() - 1]->x == snake[snake.length() - 2]->x && snake[snake.length() - 1]->y > snake[snake.length() - 2]->y)
            {
                dX = 0;
                dY = 1;
                qDebug() << "down";
            }
            if (snake[snake.length() - 1]->x == snake[snake.length() - 2]->x && snake[snake.length() - 1]->y < snake[snake.length() - 2]->y)
            {
                dX = 0;
                dY = -1;
                qDebug() << "up";
            }
            if (snake[snake.length() - 1]->x > snake[snake.length() - 2]->x && snake[snake.length() - 1]->y == snake[snake.length() - 2]->y)
            {
                dX = 1;
                dY = 0;
                qDebug() << "right";
            }
            if (snake[snake.length() - 1]->x < snake[snake.length() - 2]->x && snake[snake.length() - 1]->y == snake[snake.length() - 2]->y)
            {
                dX = -1;
                dY = 0;
                qDebug() << "left";
            }
            emit displayStepSignal(steps);                                                     // 发送步数信号
            emit displayScoreSignal(scores);                                                   // 发送分数信号
            QMessageBox::information(this, "Finished!", "Load Successfully", QMessageBox::Ok); // 显示加载成功信息框
        }
        file.close();
    }
}

void GameWidget::quitGame()
{
    this->parentWidget()->close(); // 关闭父窗口
}

void GameWidget::snakeMoveSlots()
{
    moveSnake(); // 调用移动蛇的函数
}

GameWidget::~GameWidget()
{
    delete ui; // 删除UI对象
}
