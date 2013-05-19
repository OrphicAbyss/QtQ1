#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class Matrix;
struct BSPFile;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void setMap(BSPFile *map);

protected:
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    BSPFile *map;

    Matrix *projection;
    Matrix *world;
signals:
    
public slots:
    
};

#endif // GLWIDGET_H
