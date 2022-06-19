#include "Matrice4D.h"
#include <cmath>
#include <math.h>
#include "Vec.h"

float* Matrice4D::identity()
{
    float tab[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
    };
    return tab;
}

float* Matrice4D::projection(float right, float left, float top, float bottom, float far, float near)
{
    float tab[] = {
    2.0 / (right - left), 0.f, 0.f, 0.f,
    0.f, 2.0 / (top - bottom), 0.f, 0.f,
    0.f, 0.f, -2.0 / (far - near), 0.f,
    - (right + left) / (right - left)  , -(top + bottom) / (top - bottom)  , -(far + near) / (far - near)  , 1.f
    };
    return tab;
}

float* Matrice4D::perspective(float f, float aspect, float far, float near)
{
    float tab[] = {
    f / aspect, 0.f, 0.f, 0.f,
    0.f, f, 0.f, 0.f,
    0.f, 0.f, ((far + near) / (near - far)), -1.f,
    0.f, 0.f, ((2 * near * far) / (near - far)), 0.f
    };
    return tab;
}

float* Matrice4D::translation(float x, float y, float z)
{
    float tab[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    x  , y  , z  , 1.f
    };
    return tab;
}

float* Matrice4D::rotation(float x, float y, float z)
{
    float tabx[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, cos(x), sin(x), 0.f,
    0.f, -sin(x), cos(x), 0.f,
    0.f, 0.f, 0.f, 1.f
    };
    float taby[] = {
    cos(x), 0.f, -sin(x), 0.f,
    0.f, 0.f, 0.f, 0.f,
    sin(x), 0.f, cos(x), 0.f,
    0.f, 0.f, 0.f, 1.f
    };
    float tabz[] = {
    cos(x), sin(x), 0.f, 0.f,
    -sin(x), cos(x), 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 1.f
    };
    float* tabxy = multiplie(tabx, taby);
    return multiplie(tabxy, tabz);
}

float* Matrice4D::scale(float x, float y, float z)
{
    float tab[] = {
    x  , 0.f, 0.f, 0.f,
    0.f, y  , 0.f, 0.f,
    0.f, 0.f, z  , 0.f,
    0.f, 0.f, 0.f, 1.f
    };
    return tab;
}

float* Matrice4D::multiplie(float* a, float* b)
{
    float tab[] = {
        a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],       a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],      a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],      a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
        a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],       a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],      a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],      a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
        a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],      a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],     a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],     a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
        a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],      a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],     a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],     a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15],
    };
    return tab;
}

float* Matrice4D::add(float* a, float* b)
{
    float tab[16];
    for (int i = 0; i < 16; i++) {
        tab[i] = a[i] + b[i];
    }

    return tab;
}

float* Matrice4D::LookAt(vec3 position, vec3 target, vec3 up)
{
    vec3 forward = { -(target.x - position.x), -(target.y - position.y), -(target.y - position.y) };
    forward = Vec::normaliseVec3(forward);

    vec3 right = Vec::crossProductVec3(up, forward);

    up = Vec::crossProductVec3(forward, right);

    float tab[] = {
    right.x, up.x, forward.x, 0.f,
    right.y, up.y, forward.y, 0.f,
    right.z, up.z, forward.z, 0.f,
    -Vec::dotProductVec3(right,position), -Vec::dotProductVec3(up,position), -Vec::dotProductVec3(forward,position), 1.f
    };
    return tab;
}
