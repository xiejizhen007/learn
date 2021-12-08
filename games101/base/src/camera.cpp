#include "camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 position_, glm::vec3 up_, float yaw_, float pitch_) {
    position = position_;
    worldUp = up_;
    yaw = yaw_;
    pitch = pitch_;

    moveSpeed = 2.5f;
    mouseSensitivity = 0.1f;
    zoom = 45.f;
    updateCameraVector();
}

glm::mat4
Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void
Camera::cameraMovement(float deltaTime, camera::CameraMovement dir) {
    float v = moveSpeed * deltaTime;
    if (dir == camera::W) {
        position += front * v;
    } else if (dir == camera::S) {
        position -= front * v;
    } else if (dir == camera::A) {
        position -= right * v;
    } else if (dir == camera::D) {
        position += right * v;
    }
}

void
Camera::cameraMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw     += xoffset;
    pitch   += yoffset;

    if (pitch > 60.f) {
        pitch = 60.f;
    } else if (pitch < -60.f) {
        pitch = -60.f;
    }

    updateCameraVector();
}

void
Camera::updateCameraVector() {
    glm::vec3 front_;
    front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_.y = sin(glm::radians(pitch));
    front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front_);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}