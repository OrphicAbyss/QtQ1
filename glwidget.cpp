#include "glwidget.h"

#include <QInputEvent>

#include "matrix.h"
#include "bspfile.h"
#include "gametime.h"

bool first = true;
QVector<QRgb> colorPalette;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    map = NULL;
    projection = new Matrix();
    world = new Matrix();
}

void GLWidget::setPaletteFile(unsigned char *fileData) {
    int length = 255 * 3;
    QVector<QRgb> colors(0);

    for (int i=0; i<length; i+=3) {
        colors.append(qRgb(fileData[i+0], fileData[i+1], fileData[i+2]));
    }

    colorPalette = colors;
}

void GLWidget::setMap(BSPFile *map)
{
    this->map = map;

    first = true;
    BSPMipTexEntries *entries = map->getTexturesEntries();
    this->mapTextures = (GLuint *)malloc(sizeof(GLuint) * entries->numMipTex);

    for (int i=0; i<entries->numMipTex; i++) {
        BSPMipTex *entry = (BSPMipTex *)((char *)entries + entries->dataofs[i]);
        QImage texture((unsigned char *)entry + entry->offsets[0], entry->width, entry->height, QImage::Format_Indexed8);
        texture.setColorTable(colorPalette);
        this->mapTextures[i] = bindTexture(texture, GL_TEXTURE_2D, GL_RGBA);
        qDebug("Texture: %s (%d x %d) %x id: %u", entry->name, entry->height, entry->width, texture.pixel(0,0), this->mapTextures[i]);
    }
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void GLWidget::paintGL()
{
    //qDebug("Frame");

    world->setToIdentity();
    //double seconds = GameTime::instance()->getTime();

    world->translate(0,-2048.0,0);
    world->rotate(45, 0.0f, 0.0f, 1.0f);
    //world->rotate(seconds * 3.60, 1.0, 0.0, 0.0);
    //world->rotate(seconds * 4.60, 0.0, 0.0, 1.0);
    glLoadMatrixf(world->data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (map != NULL) {
//RENDER ALL FACES
        int faceCount = map->countOfFaces();
        BSPFace *faces = map->getFaces();
        int *faceEdges = map->getEdgeList();
        BSPEdge *edges = map->getEdges();
        Vec3 *vertices = map->getVertices();
        BSPTextureInfo *textureInfos = map->getTextureInfos();
        int lastId = 0, thisId = 0;
        Vec3 fan[3];


        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        for (int i=0; i<faceCount; i++) {
            BSPFace *face = &faces[i];
            BSPTextureInfo textInfo = textureInfos[face->textureInfo];

            if (first) {
//                qDebug("Texture Info Id: %u", face->textureInfo);
//                qDebug("MipTexture Id: %u", textInfo.textureId);
//                qDebug("MipTexture name: %s", map->getMipTexture(textInfo.textureId)->name);
            }
//            GLuint texId = this->mapTextures[textureInfos[face->textureInfo].textureId];
            short firstEdge = face->firstEdge;
            int edgeCount = face->countOfEdges;

            glEnable(GL_TEXTURE_2D);
            thisId = this->mapTextures[textInfo.textureId];
//            if (thisId != lastId) {
                glBindTexture(GL_TEXTURE_2D, thisId);
//                lastId = thisId;
//            }
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

                        scalar s = Vec3::dot(&fan[k], &textInfo.vectorS) + textInfo.distS;
                        s /= map->getMipTexture(textInfo.textureId)->width;

                        scalar t = Vec3::dot(&fan[k], &textInfo.vectorT) + textInfo.distT;
                        t /= map->getMipTexture(textInfo.textureId)->height;

                        glColor4f(1.0f,green,blue,1.0f);
                        glTexCoord2f(s, t);
                        glVertex3fv(vertex);
                    }
                }
            }
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    first = false;
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    double yFOV = 45.0;
    double aspect = (double)width / (double)height;
    projection->setToIdentity();
    projection->perspective(yFOV / aspect, aspect, 5, 8192);
    projection->rotate(90.0f, 1.0f, 0.0f, 0.0f);
    projection->scale(1.0f, 1.0f, -1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection->data());

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::keyPressEvent (QKeyEvent *event)
{
    qDebug("Key press: %c", event->key());

    QGLWidget::keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    qDebug("Key release: %c", event->key());

    QGLWidget::keyReleaseEvent(event);
}
