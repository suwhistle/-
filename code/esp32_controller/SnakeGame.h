#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Keys.h" // 引用您的按键定义

// 游戏配置
#define SNAKE_BLOCK_SIZE 10  // 蛇身每个方块的大小(像素)
#define SNAKE_SPEED_MS 150   // 蛇移动速度 (越小越快)
#define MAX_SNAKE_LENGTH 100 // 蛇的最大长度

// 方向枚举
enum SnakeDir { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_STOP };

// 坐标结构体
struct Point {
    int x;
    int y;
};

class SnakeGame {
public:
    // 构造函数：接收 Sprite 指针和 Keys 指针
    SnakeGame(TFT_eSprite* tft, Keys* keys);

    void start();  // 开始/重置游戏
    void update(); // 游戏主循环 logic
    void stop();   // 退出游戏

    bool isRunning(); // 检查游戏是否正在运行

private:
    TFT_eSprite* _tft; // 修改为 Sprite 指针
    Keys* _keys;

    bool _gameRunning = false;
    bool _gameOver = false;
    
    // 蛇的状态
    Point _snake[MAX_SNAKE_LENGTH]; 
    int _snakeLength;               
    SnakeDir _currentDir;           
    SnakeDir _nextDir;              
    
    // 食物
    Point _food;

    // 屏幕边界
    int _gridWidth;
    int _gridHeight;

    // 计时器
    unsigned long _lastMoveTime;

    // 内部函数
    void spawnFood();
    void drawGameScreen();
    void handleInput();
    void move();
    void gameOver();
    void drawBlock(int x, int y, uint16_t color);
};