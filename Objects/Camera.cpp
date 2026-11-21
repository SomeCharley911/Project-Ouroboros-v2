#include "Camera.h"

#include <algorithm>
#include <cmath>

Camera::Camera(const std::array<float, 3>& position)
{
  m_position = position;
}

void Camera::rotate(float yaw_offset, float pitch_offset)
{
  m_yaw += yaw_offset;
  m_pitch = std::clamp(m_pitch + pitch_offset, -max_pitch, max_pitch);
}

void Camera::move(float forward, float strafe, float vertical, float delta_time)
{
  const float horizontal_length = std::sqrt(forward * forward + strafe * strafe);
  if (horizontal_length > 0.0F) {
    forward /= horizontal_length;
    strafe /= horizontal_length;
  }

  const float yaw_cos = std::cos(m_yaw);
  const float yaw_sin = std::sin(m_yaw);
  const float distance = movement_speed * delta_time;
  m_position[0] += (yaw_sin * forward + yaw_cos * strafe) * distance;
  m_position[1] += vertical * distance;
  m_position[2] += (yaw_cos * forward - yaw_sin * strafe) * distance;
}

std::array<float, 3> Camera::toCameraSpace(const std::array<float, 3>& point) const
{
  const float relative_x = point[0] - m_position[0];
  const float relative_y = point[1] - m_position[1];
  const float relative_z = point[2] - m_position[2];
  const float yaw_cos = std::cos(m_yaw);
  const float yaw_sin = std::sin(m_yaw);
  const float pitch_cos = std::cos(m_pitch);
  const float pitch_sin = std::sin(m_pitch);
  const float yaw_x = yaw_cos * relative_x - yaw_sin * relative_z;
  const float yaw_z = yaw_sin * relative_x + yaw_cos * relative_z;

  return {
      yaw_x,
      pitch_cos * relative_y - pitch_sin * yaw_z,
      pitch_sin * relative_y + pitch_cos * yaw_z,
  };
}
