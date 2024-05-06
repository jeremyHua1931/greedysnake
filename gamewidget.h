#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <ctime>
#include <QPushButton>
#include <QFont>
#include <QLineEdit>
#include <QLCDNumber>

#define MAX_X 40
#define MAX_Y 40
#define Label_Size 20

// 定义枚举类型Labeltype，表示标签的类型
enum Labeltype
{
    bg_label,     // 背景标签
    snake_label,  // 蛇标签
    food_label,   // 食物标签
    border_label, // 边界标签
};

// 定义结构体Snake，表示蛇的信息
struct Snake
{
    QLabel *label; // QLabel指针，表示蛇的标签
    int type;      // 蛇的类型，使用Labeltype枚举类型表示
    int x;         // 蛇的x坐标
    int y;         // 蛇的y坐标
};

namespace Ui
{
    class GameWidget;
}

// GameWidget类，继承自QWidget
class GameWidget : public QWidget
{
    Q_OBJECT
protected:
    void keyPressEvent(QKeyEvent *e);         // 键盘按下事件处理函数
    void mousePressEvent(QMouseEvent *event); // 鼠标点击事件处理函数
public:
    explicit GameWidget(QWidget *parent = nullptr); // 构造函数
    ~GameWidget();                                  // 析构函数
    void initGame();                                // 初始化游戏
    void initBorder();                              // 初始化边界
    void initSnake();                               // 初始化蛇
    void moveSnake();                               // 移动蛇
    void createFood();                              // 创建食物
    void startGame(double);                         // 开始游戏
    void pauseGame();                               // 暂停游戏
    void continueGame(double);                      // 继续游戏
    void enableCreate();                            // 允许创建
    void saveGame();                                // 保存游戏
    void loadGame();                                // 加载游戏
    void restartGame();                             // 重新开始游戏
    void quitGame();                                // 退出游戏
    void gameOver();                                // 游戏结束
signals:
    void gameOverSignal();        // 游戏结束信号
    void displayStepSignal(int);  // 显示步数信号
    void displayScoreSignal(int); // 显示分数信号
private:
    int steps = 0;                  // 步数
    int scores = 0;                 // 分数
    double speed;                   // 速度
    Ui::GameWidget *ui;             // UI指针
    Snake *map_label[MAX_X][MAX_Y]; // 地图标签数组
    QTimer timer;                   // 计时器
    QList<Snake *> snake;           // 蛇
    Snake *head;                    // 蛇头
    Snake *tail;                    // 蛇尾
    int dX, dY;                     // 移动方向
    bool canCreat;                  // 是否可以创建
    bool clicked[MAX_X][MAX_Y];     // 点击状态
    bool pressed;                   // 按下状态
    bool canMove;                   // 是否可以移动
public slots:
    void snakeMoveSlots(); // 蛇移动槽函数
};

#endif // GAMEWIDGET_H
