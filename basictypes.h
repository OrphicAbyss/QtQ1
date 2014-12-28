#ifndef BASICTYPES_H
#define BASICTYPES_H

typedef float scalar;        // Scalar value,

struct Vec3                 // Vector or Position
{
    union {
        struct {
            scalar x;                  // horizontal
            scalar y;                  // horizontal
            scalar z;                  // vertical
        };
        scalar values[3];
    };

    scalar *getValues();
    void set(const Vec3 *values);
    static scalar dot(Vec3 *vector1, Vec3 *vector2);
};

#endif // BASICTYPES_H
