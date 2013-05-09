#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile();
    void closeFile();
    void renderFrame();
    void updateFPS();
    
private:
    Ui::MainWindow *ui;
    QByteArray data;
    QTimer *timer;
    QTimer *fpsTimer;
};

#endif // MAINWINDOW_H
