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

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
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

        Vec3 fan[3];

        for (int i=0; i<faceCount; i++) {
            BSPFace *face = &faces[i];
            short firstEdge = face->firstEdge;
            int edgeCount = face->countOfEdges;

            glBegin(GL_TRIANGLES);
            for (int j=0; j<edgeCount; j++) {
                int edgeIndex = faceEdges[firstEdge + j];
                BSPEdge *edge;
                Vec3 *vertex1;

                if (0 > edgeIndex) {
                    edge = &edges[-edgeIndex];
                    vertex1 = &vertices[edge->vertex2];
                } else {
                    edge = &edges[edgeIndex];
                    vertex1 = &vertices[edge->vertex1];
                }

                if (j < 2) {
                    fan[j].set(vertex1);
                } else {
                    if (j > 2) {
                        fan[1].set(&fan[2]);
                    }
                    fan[2].set(vertex1);

                    for (int k=0; k<3; k++) {
                        float *vertex = fan[k].values;
                        float green = vertex[0] / 1024.0f;
                        float blue = vertex[1] / 1024.0f;

                        glColor4f(1.0f,green,blue,1.0f);
                        glVertex3fv(vertex);
                    }
                }
            }
            glEnd();
        }
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
