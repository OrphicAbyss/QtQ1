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
