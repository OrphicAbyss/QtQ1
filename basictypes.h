#ifndef BASICTYPES_H
#define BASICTYPES_H

typedef float scalar;        // Scalar value,

typedef struct                 // Vector or Position
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
} Vec3;

#endif // BASICTYPES_H
