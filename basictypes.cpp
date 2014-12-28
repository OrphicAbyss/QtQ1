#include "basictypes.h"

scalar *Vec3::getValues()
{
    return &values[0];
}

void Vec3::set(const Vec3 *values)
{
    x = values->x;
    y = values->y;
    z = values->z;
}

scalar Vec3::dot(Vec3 *vector1, Vec3 *vector2) {
    return vector1->x * vector2->x + vector1->y * vector2->y + vector1->z * vector2->z;
}
