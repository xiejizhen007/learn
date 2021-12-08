#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace camera
{
    enum CameraMovement {
        W,
        A,
        S,
        D
    };
} // namespace camera


class Camera {
public:
    // camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float moveSpeed;
    float mouseSensitivity;
    float zoom;

    Camera() {}
    Camera(glm::vec3 position_, glm::vec3 up_, float yaw_, float pitch_);

    glm::mat4 getViewMatrix();
    void cameraMovement(float deltaTime, camera::CameraMovement dir);
    void cameraMouseMovement(float xoffset, float yoffset);
private:
    void updateCameraVector();
};

#endif  // CAMERA_H