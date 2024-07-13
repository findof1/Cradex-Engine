#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;

  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

  inline glm::mat4 GetViewMatrix()
  {
    updateCameraVectors();
    return glm::lookAt(Position, Position + Front, Up);
  }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime);

  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

  void ProcessMouseScroll(float yoffset);

private:
  void updateCameraVectors();
};
#endif