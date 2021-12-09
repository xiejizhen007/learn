#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <glm/glm.hpp>

// 顶点属性
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

// 纹理属性
struct Texture {
    unsigned int id;    
};

// 网格体
class Mesh {
public:
    // 顶点缓冲对象
    unsigned int VAO;
    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;
    std::vector<Texture>        textures;

    // 先载入数据
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(class Shader* shader);
    
    // 后载入数据
    Mesh() {};
    bool loadMesh(const std::string& path);
    bool loadTexture(const std::string& path);

private:
    // 顶点缓冲对象，索引缓冲对象
    unsigned int VBO, EBO;

    void setupMesh();
};

#endif  // MESH_H