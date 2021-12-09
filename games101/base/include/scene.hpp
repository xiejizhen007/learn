#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"

#include <string>

class Scene {
public:
    bool initScene();
    void drawScene();

    static Scene* getInstance() {
        static Scene s;
        return &s;
    }

    float lastX, lastY;
    bool firstMouse;

    Camera* camera;

    GLFWwindow* window;
private:
    Scene(const std::string& title = "title", int w = 960, int h = 720);
    // 窗口属性
    int width, height;
    std::string title;


    float deltaTime;
    float lastTime;


    // 按键输入
    void windowInput();

};

void framebufferCallBack(GLFWwindow* window, int w, int h);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);

#endif  // SCENE_H