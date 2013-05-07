#include "glwidget.h"

#include "matrix.h"
#include "bspfile.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    map = NULL;
    projection = new Matrix();
    world = new Matrix();
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
    glLoadMatrixf(world->data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (map != NULL) {
//RENDER ALL FACES
        int faceCount = map->countOfFaces();
        BSPFace *faces = map->getFaces();
        int *faceEdges = map->getEdgeList();
        BSPEdge *edges = map->getEdges();
        Vec3 *vertices = map->getVertices();

        glBegin(GL_LINES);
        for (int i=0; i<faceCount; i++) {
            BSPFace *face = &faces[i];
            short firstEdge = face->firstEdge;
            int edgeCount = face->countOfEdges;

            for (int j=0; j<edgeCount; j++) {
                int edgeIndex = faceEdges[firstEdge + j];
                BSPEdge *edge;
                float *vertex1, *vertex2;

                if (0 > edgeIndex) {
                    edge = &edges[-edgeIndex];
                    vertex2 = &vertices[edge->vertex1].values[0];
                    vertex1 = &vertices[edge->vertex2].values[0];
                } else {
                    edge = &edges[edgeIndex];
                    vertex1 = &vertices[edge->vertex1].values[0];
                    vertex2 = &vertices[edge->vertex2].values[0];
                }

                float green = vertex1[0] / 1024.0f;
                float blue = vertex1[1] / 1024.0f;

                glColor4f(1.0f,green,blue,1.0f);
                glVertex3fv(vertex1);

                green = vertex2[0] / 1024.0f;
                blue = vertex2[1] / 1024.0f;

                glColor4f(1.0f,green,blue,1.0f);
                glVertex3fv(vertex2);
            }
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
