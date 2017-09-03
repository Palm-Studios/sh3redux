/** @file
 *
 *  Implementation of camera.hpp
 *
 *  @copyright 2017 Palm Studios
 *
 *  @date 1-9-2017
 *
 *  @author Jesse Buhagiar
 */
#include <cmath>
#include "SH3/camera/camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <boost/algorithm/clamp.hpp>

static constexpr glm::vec3 worldUp = glm::vec3(0, 1, 0); /**< Which axis is considered 'world up'. */

sh3::camera::Camera::Camera(const glm::vec3& pos, angle<float> fov, float aspect, float zNear, float zFar)
: camPos(pos), camFov(fov), aRatio(aspect), camNear(zNear), camFar(zFar),
  camFront(glm::vec3(0.0f, 0.0f, -1.0f)), camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
  camPitch(angle<float>::FromDegrees(0.0f)),
  camYaw(angle<float>::FromDegrees(-90.0f)),
  camMode(MODE::FIRST_PERSON)
{
    RecalculatePerspective();
}

void sh3::camera::Camera::RecalculatePerspective()
{
    glm::vec3 front;
    front.x = std::cos(camPitch.AsRadians()) * std::cos(camYaw.AsRadians());
    front.y = std::sin(camPitch.AsRadians());
    front.z = std::cos(camPitch.AsRadians()) * std::sin(camYaw.AsRadians());
    camFront = glm::normalize(front);

    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight, camFront));

    //vMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
    pMatrix = glm::perspective(camFov.AsRadians(), aRatio, camNear, camFar) * glm::lookAt(camPos, camPos + camFront, camUp); // Set our perspective matrix
}

const glm::mat4& sh3::camera::Camera::GetProjectionMatrix() const
{
    return pMatrix;
}

void sh3::camera::Camera::SetPosition(const glm::vec3& pos)
{
    camPos = pos;
}

void sh3::camera::Camera::Translate(const glm::vec3& trans)
{
    if(camMode == MODE::FIRST_PERSON)
    {
        camPos += trans;
        RecalculatePerspective();
    }
}

void sh3::camera::Camera::Translate(const float factor)
{
    if(camMode == MODE::FIRST_PERSON)
    {
        camPos += camFront * factor;
        RecalculatePerspective();
    }
}

void sh3::camera::Camera::SetFOV(angle<float> fov)
{
    camFov = fov;
    RecalculatePerspective();
}

void sh3::camera::Camera::SetMode(sh3::camera::MODE mode)
{
    camMode = mode;
}

void sh3::camera::Camera::AddYaw(angle<float> yaw)
{
    camYaw += yaw;
    RecalculatePerspective();
}

void sh3::camera::Camera::AddPitch(angle<float> pitch)
{
    //FIXME: This clamp causes a weird perspective when clamped between [-90, 90], which could be due to the way we are calculating the right vector (@ref camRight)
    camPitch = boost::algorithm::clamp(camPitch + pitch, angle<float>::FromDegrees(-89.0f), angle<float>::FromDegrees(89.0f));
    RecalculatePerspective();
}

void sh3::camera::Camera::LookAt(const glm::vec3& look)
{
    glm::vec3 lookDir = camPos - look;
    camYaw += camYaw.FromRadians(-std::asin(lookDir.x / (std::sqrt((lookDir.x * lookDir.x) + (lookDir.z * lookDir.z)))));
    camPitch += camPitch.FromRadians(-std::atan(lookDir.y/lookDir.z));
    RecalculatePerspective();
}

float sh3::camera::Camera::GetX() const
{
    return camPos.x;
}

float sh3::camera::Camera::GetY() const
{
    return camPos.y;
}

float sh3::camera::Camera::GetZ() const
{
    return camPos.z;
}

angle<float> sh3::camera::Camera::GetYaw() const
{
    return camYaw;
}

angle<float> sh3::camera::Camera::GetPitch() const
{
    return camPitch;
}


 
