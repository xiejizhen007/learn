#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Scene {
public:
    Scene(const std::string& title = "title", int w = 960, int h = 720);
    bool initScene();
    void drawScene();
private:
    // 窗口属性
    int width, height;
    std::string title;
    GLFWwindow* window;

    // 按键输入
    void windowInput();
};

#endif  // SCENE_H