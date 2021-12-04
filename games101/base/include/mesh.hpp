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
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(class Shader* shader);
private:
    // 顶点缓冲对象，索引缓冲对象
    unsigned int VBO, EBO;

    void setupMesh();
};
#endif  // MESH_H