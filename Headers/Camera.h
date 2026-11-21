#ifndef CAMERA_H
#define CAMERA_H

#include <array>

class Camera {
public:
  /**
   * @brief Construct a new camera object with the provided position
   *
   * @param array<float, 3> The initial position of the camera
   */
  explicit Camera(const std::array<float, 3>& position = {});
  /**
   * @brief Rotate the camera by the provided offsets
   *
   * @param float The offset to rotate the camera by in the yaw axis
   * @param float The offset to rotate the camera by in the pitch axis
   */
  void rotate(float yaw_offset, float pitch_offset);
  /**
   * @brief Move the camera by the provided amounts
   *
   * @param float The amount to move the camera forward
   * @param float The amount to move the camera left/right
   * @param float The amount to move the camera up/down
   * @param float The amount of time that has passed since the last frame
   */
  void move(float forward, float strafe, float vertical, float delta_time);
  /**
   * @brief Convert a point from world space to camera space
   *
   * @param array<float, 3> The point in world space
   *
   * @return array<float, 3> The point in camera space
   */
  std::array<float, 3> toCameraSpace(const std::array<float, 3>& point) const;

private:
  std::array<float, 3> m_position;   /// Current position of the camera
  float m_yaw = 0.0F;                /// Current yaw of the camera
  float m_pitch = 0.0F;              /// Current pitch of the camera
  const float movement_speed = 3.0F; /// Speed at which the camera moves
  const float max_pitch = 1.5F;      /// Maximum pitch angle of the camera
};

#endif // CAMERA_H
