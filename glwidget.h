#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class QMatrix4x4;
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

private:
    BSPFile *map;

    QMatrix4x4 *projection;
    QMatrix4x4 *world;
signals:
    
public slots:
    
};

#endif // GLWIDGET_H
