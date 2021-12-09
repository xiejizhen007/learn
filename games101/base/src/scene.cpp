#include "scene.hpp"
#include "mesh.hpp"
#include "OBJ_Loader.h"
#include "OBJLoader.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene(const std::string& title, int w, int h): title(title), width(w), height(h) {
    window = nullptr;
    camera = nullptr;

    deltaTime = 0.f;
    lastTime = 0.f;

    lastX = width / 2;
    lastY = height / 2;
    firstMouse = true;
}

bool
Scene::initScene() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferCallBack);
    glfwSetCursorPosCallback(window, mouseCallBack);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    camera = new Camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f);
    return true;
}

void 
Scene::drawScene() {
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    std::vector<Mesh*> meshs;

    obj::Loader Loader;
    // bool loadOut = Loader.loadFile("../models/85-cottage_obj/cottage_obj.obj");
    bool loadOut = Loader.loadFile("../models/spot/spot_triangulated_good.obj");
    // bool loadOut = Loader.loadFile("../models/Pallone/Ball OBJ.obj");
    // bool loadOut = Loader.loadFile("../models/Sting-Sword-lowpoly.obj");
    if (!loadOut) {
        std::cout << "failed to load obj" << std::endl;
    }

    for (auto vv: Loader.loadedVertices) {
        Vertex v;
        v.position.x = vv.position.x;
        v.position.y = vv.position.y;
        v.position.z = vv.position.z;

        v.texCoord.x = vv.texCoord.x;
        v.texCoord.y = vv.texCoord.y;

        v.normal.x = vv.normal.x;
        v.normal.y = vv.normal.y;
        v.normal.z = vv.normal.z;

        vertices.push_back(v);
    }

    indices.insert(indices.end(), Loader.loadedIndices.begin(), Loader.loadedIndices.end());
    // texture
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int w, h, c;
    unsigned char *data = stbi_load("../models/spot/spot_texture.png", &w, &h, &c, 0);
    if (data) {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "obj no texture" << std::endl;
        stbi_image_free(data);
    }

    Texture t;
    t.id = textureID;
    // textures.push_back(t);

    Mesh* newMesh = new Mesh(vertices, indices, textures);
    meshs.push_back(newMesh);

    // shader
    Shader *shader = new Shader("../shader/vertex.shader", "../shader/fragment.shader", nullptr);
    // Shader *shader = new Shader("../shader/baseLight.vertex.shader", "../shader/baseLight.fragment.shader", nullptr);
    Shader *shaderLight = new Shader("../shader/lightVertex.shader", "../shader/lightFragment.shader", nullptr);

    float verticesLight[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        float curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;

        windowInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::rotate(model, glm::radians(-55.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view  = camera->getViewMatrix();
        // view = camera->GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader->ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader->setMat4("projection", projection);
        shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader->setVec3("lightPos", 3.0f, 3.0f, -5.f);

        for (auto mesh: meshs) {
            mesh->draw(shader);
        }

        shaderLight->use();
        shaderLight->setMat4("projection", projection);
        shaderLight->setMat4("view", view);
        model = glm::translate(model, glm::vec3(3.f, 3.f, -5.f));
        shaderLight->setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete camera;
    return;
}

// private

void 
Scene::windowInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 

    // camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->cameraMovement(deltaTime, camera::W);
        // camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->cameraMovement(deltaTime, camera::A);
        // camera->ProcessKeyboard(LEFT, deltaTime);
    } 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->cameraMovement(deltaTime, camera::S);
        // camera->ProcessKeyboard(BACKWARD, deltaTime);
    } 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->cameraMovement(deltaTime, camera::D);
        // camera->ProcessKeyboard(RIGHT, deltaTime);
    }
}

void 
framebufferCallBack(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void
mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
    auto s = Scene::getInstance();
    if (s->firstMouse) {
        s->lastX = xpos;
        s->lastY = ypos;
        s->firstMouse = false;
    }

    float xoffset = xpos - s->lastX;
    float yoffset = s->lastY - ypos; 

    s->lastX = xpos;
    s->lastY = ypos;

    s->camera->cameraMouseMovement(xoffset, yoffset);
}