#include "bspfile.h"

bool BSPFile::isSupportedFile()
{
    return this->version == 29;
}

BSPModel *BSPFile::getModels()
{
    return (BSPModel *)((char *)this + this->models.offset);
}

int BSPFile::countOfModels()
{
    return this->models.size / sizeof(BSPModel);
}

char *BSPFile::getEntities()
{
    return (char *)this + this->entities.offset;
}

int BSPFile::entityStringSize()
{
    return this->entities.size;
}

Vec3 *BSPFile::getVertices()
{
    return (Vec3 *)((char *)this + this->vertices.offset);
}

int BSPFile::countOfVertices()
{
    return this->vertices.size / sizeof(Vec3);
}

BSPEdge *BSPFile::getEdges()
{
    return (BSPEdge *)((char *)this + this->edges.offset);
}

int BSPFile::countOfEdges()
{
    return this->edges.size / sizeof(BSPEdge);
}

BSPFace *BSPFile::getFaces()
{
    return (BSPFace *)((char *)this + this->faces.offset);
}

int BSPFile::countOfFaces()
{
    return this->faces.size / sizeof(BSPFace);
}

BSPTextureInfo *BSPFile::getTextureInfos()
{
    return (BSPTextureInfo *)((char *)this + this->texinfo.offset);
}

int BSPFile::countOfTextureInfos()
{
    return this->texinfo.size / sizeof(BSPTextureInfo);
}

int *BSPFile::getEdgeList()
{
    return (int *)((char *)this + this->listOfEdges.offset);
}

short BSPFace::getEdge(int edge)
{
    short first = firstEdge;

    if (0 > first) {
        // reverse edge direction if negative values
        return (countOfEdges - 1) - edge - first;
    } else {
        return first + edge;
    }
}

BSPMipTexEntries *BSPFile::getTexturesEntries()
{
    return (BSPMipTexEntries *)((char *)this + this->miptex.offset);
}
