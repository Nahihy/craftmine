#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gl3df {

  enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
  };

  constexpr float YAW = -90.0f;
  constexpr float PITCH = 0.0f;
  constexpr float SPEED = 2.5f;
  constexpr float SENSITIVITY = 0.1f;
  constexpr float ZOOM = 45.0f;


  class Camera {
    public:
      glm::vec3 position;
      glm::vec3 front;
      glm::vec3 up;
      glm::vec3 right;
      glm::vec3 worldup;
      float yaw;
      float pitch;
      float movementSpeed;
      float mouseSensitivity;
      float zoom;

      Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
             float yaw = YAW, float pitch = PITCH, float movementSpeed = SPEED, float mouseSensitivity = SENSITIVITY,
             float zoom = ZOOM) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(movementSpeed),
             mouseSensitivity(mouseSensitivity), zoom(zoom), position(position), worldup(up),
            yaw(yaw), pitch(pitch) {updateCameraVectors();}

      Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch,
             float movementSpeed = SPEED, float mouseSensitivity = SENSITIVITY, float zoom = ZOOM) :
             front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(movementSpeed), mouseSensitivity(mouseSensitivity),
             zoom(zoom), position(glm::vec3(posX, posY, posZ)), worldup(glm::vec3(upX, upY, upZ)),
             yaw(yaw), pitch(pitch) {updateCameraVectors();}

      glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
      }

      void processKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        glm::vec3 flatFront = glm::normalize(glm::vec3(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw))));

        if (direction == FORWARD)
          position += flatFront * velocity;
        if (direction == BACKWARD)
          position -= flatFront * velocity;
        if (direction == LEFT)
          position -= right * velocity;
        if (direction == RIGHT)
          position += right * velocity;
        if (direction == UP)
          position += worldup * velocity;
        if (direction == DOWN)
          position -= worldup * velocity;
      }

      void processMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainpitch) {
          if (pitch > 89.0f)
            pitch = 89.0f;
          if (pitch < -89.0f)
            pitch = -89.0f;
        }

        updateCameraVectors();
      }

      void processMouseScroll(float yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
          zoom = 1.0f;
        if (zoom > 45.0f)
          zoom = 45.0f;
      }

    private:
      void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(front, worldup));
        this->up = glm::normalize(glm::cross(right, front));
      }
  };

}
