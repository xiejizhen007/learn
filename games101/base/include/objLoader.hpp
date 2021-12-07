// obj 文件加载器
// 目前只提取需要用到的数据

#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace obj {
    struct Vector2 {
        float x, y;     // position

        // constructor
        Vector2() {
            x = 0.f;
            y = 0.f;
        }
        // 
        Vector2(float X, float Y) {
            x = X;
            y = Y;
        }
        // 
        bool operator==(const Vector2& v) const {
            return this->x == v.x && this->y == v.y;
        }

        bool operator!=(const Vector2& v) const {
            return !(this->x == v.x && this->y == v.y);
        }

        Vector2 operator+(const Vector2& v) const {
            return Vector2(
                this->x + v.x,
                this->y + v.y
            );
        }

        Vector2 operator-(const Vector2& v) const {
            return Vector2(
                this->x - v.x,
                this->y - v.y
            );
        }

        Vector2 operator*(const float& v) const {
            return Vector2(
                this->x * v, 
                this->y * v
            );
        }

        Vector2 operator/(const float& v) const {
            return Vector2(
                this->x * v, 
                this->y * v
            );
        }
    };

    struct Vector3 {
        float x, y, z;

        Vector3() {
            x = 0.f;
            y = 0.f;
            z = 0.f;
        }

        Vector3(float X, float Y, float Z) {
            x = X;
            y = Y;
            z = Z;
        }

        bool operator==(const Vector3& v) const {
            return (this->x == v.x && this->y == v.y && this->z == v.z);
        }

        bool operator!=(const Vector3& v) const {
            return !(this->x == v.x && this->y == v.y && this->z == v.z);
        }

        Vector3 operator+(const Vector3& v) const {
            return Vector3(
                this->x + v.x,
                this->y + v.y,
                this->z + v.z
            );
        }

        Vector3 operator-(const Vector3& v) const {
            return Vector3(
                this->x - v.x,
                this->y - v.y,
                this->z - v.z
            );
        }

        Vector3 operator*(const float& val) const {
            return Vector3(
                this->x * val,
                this->y * val,
                this->z * val
            );
        }

        Vector3 operator/(const float& val) const {
            return Vector3(
                this->x / val,
                this->y / val,
                this->z / val
            );
        }
    };
    
    // 顶点属性
    struct Vertex {
        // 顶点坐标
        Vector3 position;
        // 法线
        Vector3 normal;
        // 纹理坐标
        Vector2 texCoord;
    };

    struct Material {

    };

    struct Mesh {
        std::string meshName;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material meshMaterial;
    };

    class Loader {
    public:
        // mesh object
        std::vector<Mesh> loadedMeshs;
        std::vector<Vertex> loadedVertices;
        std::vector<unsigned int> loadedIndices;
        std::vector<Material> loadedMaterials;

        // TODO: 加载模型，顺带加载 mtl 文件
        bool loadFile(std::string path);
    private:
        bool loadObjectMtl(std::string path);
    };

    namespace algorithm {
        // 字符串的第一个单词
        std::string firstToken(const std::string& line);
        // 字符串最后一个单词
        std::string tailToken(const std::string& line);
        // 按空格分割字符串
        std::vector<std::string> split(std::string line, const std::string& token);
        // 字符串转换为数字
        float str2number(const std::string& str);
    }
}