#include "Vec.h"
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

vec3 Vec::vec4ToVec3(vec4 A)
{
    return { A.x, A.y, A.z };
}

vec3 Vec::add(vec3 A, vec3 B)
{
    return { A.x + B.x,A.y + B.y,A.z + B.z };
}

vec3 Vec::multiplyByFloat(vec3 A, float B)
{
    return { A.x * B, A.y * B, A.z * B };
}

vec3 Vec::sub(vec3 A, vec3 B)
{
    return { A.x - B.x,A.y - B.y,A.z - B.z };
}
