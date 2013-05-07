#include "glwidget.h"

#include <QMatrix4x4>

#include "bspfile.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    map = NULL;
    projection = new QMatrix4x4();
    world = new QMatrix4x4();
}

void GLWidget::setMap(BSPFile *map)
{
    this->map = map;
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    qDebug("Frame");

    world->setToIdentity();
    //double seconds = GameTime::instance()->getTime();

    world->translate(0,-2048.0,0);
    world->rotate(45, 0.0f, 0.0f, 1.0f);
    //world->rotate(seconds * 36.0, 1.0, 0.0, 0.0);
    //world->rotate(seconds * 46.0, 0.0, 0.0, 1.0);
    glLoadMatrixf(world->constData());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (map != NULL) {
//RENDER ALL VERTICES
//        int vertexCount = map->countOfVertices();
//        Vec3 *vertices = map->getVertices();

//        glBegin(GL_POINTS);
//        for (int i=0; i<vertexCount; i++) {
//            float *values = &vertices[i].values[0];

//            float green = values[0] / 4096.0f;
//            float blue = values[1] / 4096.0f;

//            glColor4f(1.0f,green,blue,1.0f);
//            glVertex3fv(values);
//        }
//        glEnd();

//RENDER ALL EDGES
        int edgeCount = map->countOfEdges();
        //int vertexCount = map->countOfVertices();
        BSPEdge *edges = map->getEdges();
        Vec3 *vertices = map->getVertices();

        glBegin(GL_LINES);
        for (int i=0; i<edgeCount; i++) {
            BSPEdge *edge = &edges[i];
            float *vertex1 = &vertices[edge->vertex1].values[0];
            float *vertex2 = &vertices[edge->vertex2].values[0];

            float green = vertex1[0] / 4096.0f;
            float blue = vertex1[1] / 4096.0f;

            glColor4f(1.0f,green,blue,1.0f);
            glVertex3fv(vertex1);

            green = vertex2[0] / 4096.0f;
            blue = vertex2[1] / 4096.0f;

            glColor4f(1.0f,green,blue,1.0f);
            glVertex3fv(vertex2);
        }
        glEnd();

    }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    double yFOV = 45.0;
    double aspect = (double)width / (double)height;
    projection->setToIdentity();
    projection->perspective(yFOV / aspect, aspect, 5, 4096);
    projection->rotate(90.0f, 1.0f, 0.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection->data());

    glMatrixMode(GL_MODELVIEW);
}
