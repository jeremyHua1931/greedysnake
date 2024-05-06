#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewidget.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param parent 父窗口指针，默认为nullptr
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * @brief 析构函数
     *
     */
    ~MainWindow();
signals:

private slots:
    /**
     * @brief 开始游戏触发的槽函数
     *
     */
    void on__start_triggered();

    /**
     * @brief 暂停游戏触发的槽函数
     *
     */
    void on__pause_triggered();

    /**
     * @brief 保存游戏触发的槽函数
     *
     */
    void on__save_triggered();

    /**
     * @brief 继续游戏触发的槽函数
     *
     */
    void on__continue_triggered();

    /**
     * @brief 重新开始游戏触发的槽函数
     *
     */
    void on__restart_triggered();

    /**
     * @brief 加载游戏触发的槽函数
     *
     */
    void on__load_triggered();

    /**
     * @brief 退出游戏触发的槽函数
     *
     */
    void on__quit_triggered();

    /**
     * @brief 游戏结束槽函数
     *
     */
    void gameOverSlots();

    /**
     * @brief 点击开始按钮触发的槽函数
     *
     */
    void on_start_clicked();

    /**
     * @brief 点击加载按钮触发的槽函数
     *
     */
    void on_load_clicked();

    /**
     * @brief 点击暂停按钮触发的槽函数
     *
     */
    void on_pause_clicked();

    /**
     * @brief 点击继续按钮触发的槽函数
     *
     */
    void on_con_clicked();

    /**
     * @brief 点击重新开始按钮触发的槽函数
     *
     */
    void on_restart_clicked();

    /**
     * @brief 点击保存按钮触发的槽函数
     *
     */
    void on_save_clicked();

    /**
     * @brief 点击退出按钮触发的槽函数
     *
     */
    void on_quit_clicked();

    /**
     * @brief 显示步数的槽函数
     *
     * @param step 步数值
     */
    void displayStepSlots(int step);

    /**
     * @brief 显示分数的槽函数
     *
     * @param score 分数值
     */
    void displayScoreSlots(int score);

    /**
     * @brief 显示游戏规则
     *
     */   
    void showHelpDialog();


private:
    Ui::MainWindow *ui;
    QMediaPlayer *backgroundMusicPlayer;     // 背景音乐播放器指针
    QMediaPlaylist *backgroundMusicPlaylist; // 背景音乐播放列表指针
};

#endif // MAINWINDOW_H
