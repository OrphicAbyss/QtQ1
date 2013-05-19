#ifndef MATRIX_H
#define MATRIX_H

class QMatrix4x4;

/**
 * Provides 4x4 matrix operations.
 *
 * Currently a wrapper for a Qt class, but would need to be filled out with
 * other implementions for portability.
 */
class Matrix
{
private:
    /** Current implementation object */
    QMatrix4x4 *implObj;

public:
    Matrix();

    /** Set this matrix object to the identity object */
    void setToIdentity();

    /** Create a matrix based on the provided perspective values. */
    void perspective(float vAngle, float aspectRatio, float near, float far);

    /** Apply translate to matrix. */
    void translate(float x, float y, float z);
    /** Apply rotation to matrix. */
    void rotate(float angle, float x, float y, float z);
    /** Apply scale to matrix. */
    void scale(float x, float y, float z);

    /** Get data as an array */
    const float *data();
};

#endif // MATRIX_H
