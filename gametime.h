#ifndef GAMETIME_H
#define GAMETIME_H

#include <QObject>

class QElapsedTimer;

class GameTime : public QObject {
	Q_OBJECT
public:
	static GameTime *instance();

	GameTime();
	virtual ~GameTime();

	void newFrame();
	double getTime();
	double getFrameTime();

	int markFPS();
private:
	QElapsedTimer *curTime;

	int fps;
	double time;
	double frameTime;

	static GameTime *instanceObj;
};

extern GameTime gameTime;

#endif // GAMETIME_H
