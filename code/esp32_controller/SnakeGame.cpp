#include "SnakeGame.h"

// 颜色定义
#define COL_BG      TFT_BLACK
#define COL_SNAKE   TFT_GREEN
#define COL_FOOD    TFT_RED
#define COL_TEXT    TFT_WHITE

// 构造函数
SnakeGame::SnakeGame(TFT_eSprite* tft, Keys* keys) {
    _tft = tft;
    _keys = keys;
}

void SnakeGame::start() {
    _gameRunning = true;
    _gameOver = false;
    _snakeLength = 3;
    _currentDir = DIR_RIGHT;
    _nextDir = DIR_RIGHT;
    
    // 动态计算网格大小 (依赖 Sprite 的宽高)
    _gridWidth = _tft->width() / SNAKE_BLOCK_SIZE;
    _gridHeight = _tft->height() / SNAKE_BLOCK_SIZE;

    // 初始化蛇的位置 (屏幕中间)
    int startX = _gridWidth / 2;
    int startY = _gridHeight / 2;
    for(int i=0; i<_snakeLength; i++) {
        _snake[i] = {startX - i, startY};
    }

    // 初始绘图
    _tft->fillSprite(COL_BG); // 清空 Sprite
    spawnFood();
    drawGameScreen(); 
    
    _lastMoveTime = millis();
}

void SnakeGame::stop() {
    _gameRunning = false;
    // 退出时不在这里清屏，由主程序的菜单逻辑接管
}

bool SnakeGame::isRunning() {
    return _gameRunning;
}

void SnakeGame::update() {
    if (!_gameRunning) return;

    // 1. 处理输入
    handleInput();

    // 2. 游戏结束逻辑
    if (_gameOver) {
        // 这里的按键逻辑假设 keys 已经在外部 update 过了
        // a 键重开 (假设 a=0 是按下)
        if (_keys->kvs.a == 0) { 
            start(); 
        } 
        // b 键退出
        else if (_keys->kvs.b == 0) { 
            stop();
        }
        return;
    }

    // 3. 移动逻辑 (时间控制)
    if (millis() - _lastMoveTime > SNAKE_SPEED_MS) {
        move();
        _lastMoveTime = millis();
    }
}

void SnakeGame::handleInput() {
    // 读取十字键输入 (up/down/left/right 假设按下为0)
    // 防止直接掉头
    if (_keys->kvs.up == 0 && _currentDir != DIR_DOWN) _nextDir = DIR_UP;
    else if (_keys->kvs.down == 0 && _currentDir != DIR_UP) _nextDir = DIR_DOWN;
    else if (_keys->kvs.left == 0 && _currentDir != DIR_RIGHT) _nextDir = DIR_LEFT;
    else if (_keys->kvs.right == 0 && _currentDir != DIR_LEFT) _nextDir = DIR_RIGHT;
}

void SnakeGame::move() {
    _currentDir = _nextDir; 

    // 计算新蛇头
    Point newHead = _snake[0];
    switch (_currentDir) {
        case DIR_UP:    newHead.y--; break;
        case DIR_DOWN:  newHead.y++; break;
        case DIR_LEFT:  newHead.x--; break;
        case DIR_RIGHT: newHead.x++; break;
        default: return;
    }

    // --- 碰撞检测 ---
    // 1. 撞墙
    if (newHead.x < 0 || newHead.x >= _gridWidth || newHead.y < 0 || newHead.y >= _gridHeight) {
        gameOver();
        return;
    }
    // 2. 撞自己
    for (int i = 0; i < _snakeLength; i++) {
        if (newHead.x == _snake[i].x && newHead.y == _snake[i].y) {
            gameOver();
            return;
        }
    }

    // --- 移动蛇身 ---
    Point tail = _snake[_snakeLength - 1]; // 记录旧尾巴

    // 身体后移
    for (int i = _snakeLength - 1; i > 0; i--) {
        _snake[i] = _snake[i - 1];
    }
    _snake[0] = newHead; 

    // --- 吃食物 ---
    bool ateFood = (newHead.x == _food.x && newHead.y == _food.y);
    
    // --- 绘图更新 (在 Sprite 上操作) ---
    // 画新头
    drawBlock(newHead.x, newHead.y, COL_SNAKE);
    
    if (ateFood) {
        if (_snakeLength < MAX_SNAKE_LENGTH) {
            _snakeLength++; 
            _snake[_snakeLength - 1] = tail; // 恢复尾巴作为新的一节
        }
        spawnFood(); 
        // 这里可以加 buzzer.beep()
    } else {
        // 擦除旧尾巴
        drawBlock(tail.x, tail.y, COL_BG); 
    }
}

void SnakeGame::spawnFood() {
    _food.x = random(0, _gridWidth);
    _food.y = random(0, _gridHeight);
    // 简单起见，不检查是否生成在蛇身上
    drawBlock(_food.x, _food.y, COL_FOOD);
}

void SnakeGame::drawBlock(int x, int y, uint16_t color) {
    // 使用 Sprite 的绘图函数
    _tft->fillRect(x * SNAKE_BLOCK_SIZE, y * SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE - 1, SNAKE_BLOCK_SIZE - 1, color);
}

void SnakeGame::drawGameScreen() {
    for (int i = 0; i < _snakeLength; i++) {
        drawBlock(_snake[i].x, _snake[i].y, COL_SNAKE);
    }
    drawBlock(_food.x, _food.y, COL_FOOD);
}

void SnakeGame::gameOver() {
    _gameOver = true;
    _tft->setTextColor(COL_FOOD);
    _tft->setTextDatum(MC_DATUM); // 中间对齐
    _tft->drawString("GAME OVER", _tft->width()/2, _tft->height()/2);
    
    _tft->setTextColor(COL_TEXT);
    _tft->setTextSize(1); // 稍微调小字体显示提示
    _tft->drawString("A: Retry  B: Exit", _tft->width()/2, _tft->height()/2 + 30);
    _tft->setTextSize(2); // 恢复字体大小（假设默认是2，根据实际情况调整）
}