#include "gametime.h"

#include <QElapsedTimer>
#include <QTimer>

GameTime *GameTime::instanceObj;

GameTime::GameTime() {
    curTime = new QElapsedTimer();
    curTime->start();

    time = 0;
    frameTime = 0;
    fps = 0;
}

GameTime::~GameTime() {
    delete curTime;
}

GameTime *GameTime::instance() {
    if (instanceObj == NULL)
        instanceObj = new GameTime();

    return instanceObj;
}

int GameTime::markFPS() {
    int returnVal = fps;
    fps = 0;
    return returnVal;

}

void GameTime::newFrame() {
    fps++;
    qint64 elapsed = curTime->elapsed();
    double newTime = elapsed / 1000.0;
    frameTime = newTime - time;
    time = newTime;
}

double GameTime::getFrameTime() {
    return frameTime;
}

double GameTime::getTime() {
    return time;
}
