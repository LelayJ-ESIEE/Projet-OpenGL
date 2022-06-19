#include "Vec.h"
#include <cmath>

vec3 Vec::normaliseVec3(vec3 vec)
{
    float norme = sqrt(powf(vec.x, 2.0) + powf(vec.y, 2.0) + powf(vec.z, 2.0));
    vec3 result{ vec.x / norme, vec.y / norme, vec.z / norme };
    return result;
}

vec3 Vec::crossProductVec3(vec3 A, vec3 B)
{
    vec3 result{ A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x };
    return result;
}

float Vec::dotProductVec3(vec3 A, vec3 B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}
