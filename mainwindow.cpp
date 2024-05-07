#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QtMath>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1025, 900);
    this->setWindowTitle("Greedy Snake");             // 设置窗口标题
    this->setWindowIcon(QIcon(":/icon/icon/snake.png")); // 设置窗口图标



    // 修改颜色
    ui->start->setStyleSheet("QPushButton { background-color: #4b5cc4; color: black; font-size: 16px; font-weight: bold;}"
                         "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->pause->setStyleSheet("QPushButton { background-color: #da9f6c; color: white; font-size: 16px; font-weight: bold;}"
                         "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->con->setStyleSheet("QPushButton { background-color: #da9f6c; color: white; font-size: 16px; font-weight: bold;}"
                       "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->restart->setStyleSheet("QPushButton { background-color: #00C3FF; color: black; font-size: 16px; font-weight: bold;}"
                           "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->save->setStyleSheet("QPushButton { background-color: #ebb07a; color: white; font-size: 16px; font-weight: bold;}"
                        "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->load->setStyleSheet("QPushButton { background-color: #ebb07a; color: white; font-size: 16px; font-weight: bold;}"
                        "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->quit->setStyleSheet("QPushButton { background-color: #E54046; color: black; font-size: 16px; font-weight: bold;}"
                        "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");


    ui->help->setStyleSheet("QPushButton { background-color: #4b5cc4; color: black; font-size: 14px; font-weight: bold;}"
                        "QPushButton:disabled { background-color: #C0D8D8; color: grey;}");

    ui->label->setStyleSheet("QLabel {  font-size: 16px; font-weight: bold;}");
    ui->label_2->setStyleSheet("QLabel {  font-size: 16px; font-weight: bold;}");
    ui->label_3->setStyleSheet("QLabel {  font-size: 16px; font-weight: bold;}");
    ui->spLabel->setStyleSheet("QLabel {  color: blue;font-size: 24px; font-weight: bold;}");

    ui->lcd->setStyleSheet("QLCDNumber { color: blue;  }");
    ui->lcd1->setStyleSheet("QLCDNumber { color: red;  }");



    // 初始化背景音乐播放列表和播放器
    backgroundMusicPlaylist = new QMediaPlaylist();                             // 创建背景音乐播放列表
    backgroundMusicPlaylist->addMedia(QUrl("qrc:sound/snake_background_music.wav")); // 添加背景音乐文件到播放列表
    backgroundMusicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);             // 设置循环播放模式

    backgroundMusicPlayer = new QMediaPlayer();                  // 创建背景音乐播放器
    backgroundMusicPlayer->setPlaylist(backgroundMusicPlaylist); // 设置播放器使用的播放列表
    backgroundMusicPlayer->play();                               // 播放背景音乐

    ui->game->setParent(this);                                                                       // 将游戏窗口设为当前窗口的子窗口
    ui->pause->setEnabled(false);                                                                    // 初始状态下禁用暂停按钮
    ui->con->setEnabled(false);                                                                      // 初始状态下禁用继续按钮
    ui->restart->setEnabled(false);                                                                  // 初始状态下禁用重新开始按钮
    ui->save->setEnabled(false);                                                                     // 初始状态下禁用保存按钮
    ui->lcd->display(0);                                                                             // 设置步数LCD显示为0
    ui->lcd1->display(0);                                                                            // 设置得分LCD显示为0
    QObject::connect(ui->game, SIGNAL(gameOverSignal()), this, SLOT(gameOverSlots()));               // 连接游戏结束信号和槽函数
    QObject::connect(ui->game, SIGNAL(displayStepSignal(int)), this, SLOT(displayStepSlots(int)));   // 连接显示步数信号和槽函数
    QObject::connect(ui->game, SIGNAL(displayScoreSignal(int)), this, SLOT(displayScoreSlots(int))); // 连接显示得分信号和槽函数
    QObject::connect(ui->help, &QPushButton::clicked, this, &MainWindow::showHelpDialog);
}

MainWindow::~MainWindow()
{
    delete ui;                      // 删除UI对象
    delete backgroundMusicPlayer;   // 删除背景音乐播放器对象
    delete backgroundMusicPlaylist; // 删除背景音乐播放列表对象
}

void MainWindow::on__start_triggered()
{
    int speed = ui->speed->value();              // 获取速度值
    ui->speed->setEnabled(false);                // 禁用速度调节器
    ui->game->startGame(qLn(10.5 - speed) * 50); // 开始游戏并设置速度

    ui->start->setEnabled(false);   // 禁用开始按钮
    ui->restart->setEnabled(false); // 禁用重新开始按钮
    ui->con->setEnabled(false);     // 禁用继续按钮
    ui->save->setEnabled(false);    // 禁用保存按钮
    ui->load->setEnabled(false);    // 禁用加载按钮
    ui->pause->setEnabled(true);    // 启用暂停按钮

}

void MainWindow::on__pause_triggered()
{
    ui->game->pauseGame(); // 暂停游戏

    ui->save->setEnabled(true);    // 启用保存按钮
    ui->pause->setEnabled(false);  // 禁用暂停按钮
    ui->con->setEnabled(true);     // 启用继续按钮
    ui->restart->setEnabled(true); // 启用重新开始按钮
}

void MainWindow::on__save_triggered()
{
    ui->game->saveGame(); // 保存游戏状态
}

void MainWindow::on__continue_triggered()
{
    int speed = ui->speed->value();                 // 获取速度值
    ui->game->continueGame(qLn(10.5 - speed) * 50); // 继续游戏并设置速度

    ui->con->setEnabled(false);  // 禁用继续按钮
    ui->start->setEnabled(false); // 禁用开始按钮
    ui->pause->setEnabled(true); // 启用暂停按钮
}

void MainWindow::on__restart_triggered()
{
    ui->game->restartGame();        // 重新开始游戏
    ui->con->setEnabled(false);     // 禁用继续按钮
    ui->restart->setEnabled(false); // 禁用重新开始按钮
    ui->save->setEnabled(false);    // 禁用保存按钮
    ui->load->setEnabled(true);     // 启用加载按钮
    ui->start->setEnabled(true);    // 启用开始按钮
}

void MainWindow::on__load_triggered()
{
    ui->game->loadGame();      // 加载游戏状态
    ui->con->setEnabled(true); // 启用继续按钮
}

void MainWindow::on__quit_triggered()
{
    this->close(); // 关闭应用程序
}

void MainWindow::gameOverSlots()
{
    ui->start->setEnabled(false);  // 禁用开始按钮
    ui->pause->setEnabled(false);  // 禁用暂停按钮
    ui->con->setEnabled(false);    // 禁用继续按钮
    ui->load->setEnabled(false);   // 禁用加载按钮
    ui->save->setEnabled(false);   // 禁用保存按钮
    ui->restart->setEnabled(true); // 启用重新开始按钮
    ui->speed->setEnabled(true);   // 启用速度调节器
}

void MainWindow::on_start_clicked()
{
    int speed = ui->speed->value();              // 获取速度值
    ui->speed->setEnabled(false);                // 禁用速度调节器
    ui->game->startGame(qLn(10.5 - speed) * 50); // 开始游戏并设置速度

    ui->start->setEnabled(false);   // 禁用开始按钮
    ui->restart->setEnabled(true); // 禁用重新开始按钮
    ui->con->setEnabled(false);     // 禁用继续按钮
    ui->save->setEnabled(false);    // 禁用保存按钮
    ui->load->setEnabled(false);    // 禁用加载按钮
    ui->pause->setEnabled(true);    // 启用暂停按钮
    ui->help->setEnabled(false);
}

void MainWindow::on_load_clicked()
{
    ui->game->loadGame();      // 加载游戏状态
    ui->con->setEnabled(true); // 启用继续按钮
    ui->start->setEnabled(false);   // 禁用开始按钮
     ui->help->setEnabled(true);
}

void MainWindow::on_pause_clicked()
{
    ui->game->pauseGame();         // 暂停游戏
    ui->save->setEnabled(true);    // 启用保存按钮
    ui->pause->setEnabled(false);  // 禁用暂停按钮
    ui->con->setEnabled(true);     // 启用继续按钮
    ui->restart->setEnabled(true); // 启用重新开始按钮
    ui->help->setEnabled(true);
}

void MainWindow::on_con_clicked()
{
    int speed = ui->speed->value();                 // 获取速度值
    ui->game->continueGame(qLn(10.5 - speed) * 50); // 继续游戏并设置速度
    ui->con->setEnabled(false);                     // 禁用继续按钮
    ui->pause->setEnabled(true);                    // 启用暂停按钮
    ui->load->setEnabled(false);                    // 禁用加载按钮
    ui->save->setEnabled(false);
    ui->help->setEnabled(false);
}

void MainWindow::on_restart_clicked()
{
    ui->game->restartGame();        // 重新开始游戏
    ui->speed->setEnabled(true);    // 启用速度调节器
    ui->con->setEnabled(false);     // 禁用继续按钮
    ui->restart->setEnabled(false); // 禁用重新开始按钮
    ui->save->setEnabled(false);    // 禁用保存按钮
    ui->load->setEnabled(true);     // 启用加载按钮
    ui->start->setEnabled(true);    // 启用开始按钮
    ui->pause->setEnabled(false);   // 禁用暂停按钮
    ui->help->setEnabled(true);
}

void MainWindow::on_save_clicked()
{
    ui->game->saveGame(); // 保存游戏状态
     ui->help->setEnabled(true);
}

void MainWindow::on_quit_clicked()
{
    this->close(); // 关闭应用程序
}

void MainWindow::displayStepSlots(int step)
{
    ui->lcd->display(step); // 显示步数
}

void MainWindow::displayScoreSlots(int score)
{
    ui->lcd1->display(score); // 显示得分
}

void MainWindow::showHelpDialog() {

    QMessageBox::information(this, "Game Rules",
        "Here are the rules of the game:\n\n"
        "- Avoid colliding with the walls or the snake's body.\n"
        "- Use the arrow keys to control the direction of the snake.\n"
        "- Eat the food to grow longer and increase your score.\n"
        "- The game speeds up as you eat more food.");
}
