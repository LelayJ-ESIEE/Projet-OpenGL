#include "Camera.h"

vec4 Camera::LocalToWorld()
{
    vec4 result{};
    result.x = position.x * worldMatrix[0] + position.y * worldMatrix[4] + position.z * worldMatrix[8] + position.w * worldMatrix[12];
    result.y = position.x * worldMatrix[1] + position.y * worldMatrix[5] + position.z * worldMatrix[9] + position.w * worldMatrix[13];
    result.z = position.x * worldMatrix[2] + position.y * worldMatrix[6] + position.z * worldMatrix[10] + position.w * worldMatrix[14];
    result.w = position.x * worldMatrix[3] + position.y * worldMatrix[7] + position.z * worldMatrix[11] + position.w * worldMatrix[15];
    return vec4();
}

vec4 Camera::WorldToView()
{
    vec4 result = LocalToWorld();
    result.x = position.x * viewMatrix[0] + position.y * viewMatrix[4] + position.z * viewMatrix[8] + position.w * viewMatrix[12];
    result.y = position.x * viewMatrix[1] + position.y * viewMatrix[5] + position.z * viewMatrix[9] + position.w * viewMatrix[13];
    result.z = position.x * viewMatrix[2] + position.y * viewMatrix[6] + position.z * viewMatrix[10] + position.w * viewMatrix[14];
    result.w = position.x * viewMatrix[3] + position.y * viewMatrix[7] + position.z * viewMatrix[11] + position.w * viewMatrix[15];
    return vec4();
}

vec4 Camera::ViewToClip()
{
    vec4 result = WorldToView();
    result.x = position.x * projectionMatrix[0] + position.y * projectionMatrix[4] + position.z * projectionMatrix[8] + position.w * projectionMatrix[12];
    result.y = position.x * projectionMatrix[1] + position.y * projectionMatrix[5] + position.z * projectionMatrix[9] + position.w * projectionMatrix[13];
    result.z = position.x * projectionMatrix[2] + position.y * projectionMatrix[6] + position.z * projectionMatrix[10] + position.w * projectionMatrix[14];
    result.w = position.x * projectionMatrix[3] + position.y * projectionMatrix[7] + position.z * projectionMatrix[11] + position.w * projectionMatrix[15];
    return vec4();
}
