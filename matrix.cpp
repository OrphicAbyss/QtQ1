#include "matrix.h"

#include <QMatrix4x4>

Matrix::Matrix()
{
    implObj = new QMatrix4x4();
}

void Matrix::setToIdentity()
{
    implObj->setToIdentity();
}

void Matrix::translate(float x, float y, float z)
{
    implObj->translate(x,y,z);
}

void Matrix::rotate(float angle, float x, float y, float z)
{
    implObj->rotate(angle, x, y,z);
}

void Matrix::scale(float x, float y, float z) {
    implObj->scale(x, y, z);
}

const float *Matrix::data()
{
    return implObj->constData();
}

void Matrix::perspective(float vAngle, float aspectRatio, float near, float far)
{
    implObj->perspective(vAngle, aspectRatio, near, far);
}
