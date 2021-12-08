#include "OBJLoader.hpp"
#include <iostream>
#include <sstream>      // stream
#include <fstream>      // file I/O
#include <vector>
#include <string>

namespace obj {
    bool
    Loader::loadFile(std::string path) {
        if (path.substr(path.size() - 4, 4) != ".obj") {
            std::cout << "loadFile => error path\n";
            return false;
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "loadFile => error path\n";
            return false;
        }

        // 防止二次导入
        loadedVertices.clear();
        loadedIndices.clear();
        loadedMeshs.clear();
        loadedMaterials.clear();

        std::string line; 
        std::stringstream ss;        
        std::string token;

        std::vector<Vector3> vertexPositions;
        std::vector<Vector2> vertexTexCoords;
        std::vector<Vector3> vertexNormals;

        std::vector<Vertex> vertices;           // 当前 object 的顶点
        std::vector<unsigned int> indices;      // 当前 object 的索引

        while (std::getline(file, line)) {
            ss.clear();
            ss.str(line);
            ss >> token;

            if (token == "o" || token == "g") {
                if (!vertices.empty() && !indices.empty()) {        // generate a new mesh
                    ss >> token;
                    // create new mesh
                    auto newMesh = Mesh();
                    newMesh.vertices = vertices;
                    newMesh.indices = indices;
                    newMesh.meshName = token;

                    // insert the mesh into global meshs
                    loadedMeshs.push_back(newMesh);                    
                }

                vertices.clear();
                indices.clear();
                ss >> token;
                std::cout << "object name > " << token << std::endl;
            } else if (token == "v") {          // vertex position
                Vector3 v3;
                ss >> v3.x >> v3.y >> v3.z;
                vertexPositions.push_back(v3);
            } else if (token == "vt") {         // vertex texture coordinate
                Vector2 v2;
                ss >> v2.x >> v2.y;
                vertexTexCoords.push_back(v2);
            } else if (token == "vn") {         // vertex normal
                Vector3 v3;
                ss >> v3.x >> v3.y >> v3.z;
                vertexNormals.push_back(v3);
            } else if (token == "f") {          // face 
                auto in = line.substr(line.find_first_of(' ', 0) + 1);
                // vertex's index
                auto subStr = algorithm::split(in, " ");
                // 从文件中读取索引字符串
                std::vector<std::string> vertexIndices;

                std::vector<Vertex> vVertices;          // vertex list
                std::vector<unsigned int> iIndices;     

                for (int i = 0; i < subStr.size(); i++) {
                    Vertex v;
                    auto index = algorithm::split(subStr[i], "/");
                    if (index.size() == 1) {        // only position
                        v.position = algorithm::getElement(vertexPositions, std::stoi(index[0]));
                        v.texCoord = Vector2(0.f, 0.f);
                        v.normal = Vector3(0.f, 0.f, 0.f);
                    } else if (index.size() == 2) { // position & texCoord
                        v.position = algorithm::getElement(vertexPositions, std::stoi(index[0]));
                        v.texCoord = algorithm::getElement(vertexTexCoords, std::stoi(index[1]));
                    } else if (index.size() == 3) {
                        if (index[1] == "") {       // position & & normal
                            v.position = algorithm::getElement(vertexPositions, std::stoi(index[0]));
                            v.normal = algorithm::getElement(vertexNormals, std::stoi(index[2]));
                        } else {                    // position & texCoord & normal
                            v.position = algorithm::getElement(vertexPositions, std::stoi(index[0]));
                            v.texCoord = algorithm::getElement(vertexTexCoords, std::stoi(index[1]));
                            v.normal = algorithm::getElement(vertexNormals, std::stoi(index[2]));
                        }
                    }

                    vVertices.push_back(v);                 // 当前 face 的顶点
                    vertices.push_back(v);                  // 当前对象的顶点
                    loadedVertices.push_back(v);            // 全局顶点
                }

                // 创建索引
                iIndices.push_back(0);
                iIndices.push_back(1);
                iIndices.push_back(2);

                for (int i = 3; i < subStr.size(); i++) {
                    iIndices.push_back(i - 3);
                    iIndices.push_back(i - 1);
                    iIndices.push_back(i);
                }

                for (int i = 0; i < iIndices.size(); i++) {
                    // object
                    auto idn = vertices.size() - vVertices.size() + iIndices[i];
                    indices.push_back(idn);
                    // global
                    idn = loadedVertices.size() - vVertices.size() + iIndices[i];
                    loadedIndices.push_back(idn);
                }
            }
        }

        std::cout << "vertices > " << loadedVertices.size() << std::endl;
        std::cout << "indices > " << loadedIndices.size() << std::endl;
        std::cout << "meshs > " << loadedMeshs.size() << std::endl;
        return true;
    }

    bool 
    Loader::loadObjectMtl(std::string path) {

    }    

    namespace algorithm {
        template <class T>
        const T& getElement(const std::vector<T>& element, int index) {
            int idx = 0;
            if (index < 0)
                idx = element.size() + index;
            else 
                idx = index - 1;
            return element[idx];
        }

        std::vector<std::string> split(const std::string& in, const std::string& token) {
            std::vector<std::string> ret;
            std::string tmp = "";

            for (int i = 0; i < in.size(); i++) {
                auto test = in.substr(i, token.size());
                if (test == token) {
                    ret.push_back(tmp);
                    tmp.clear();
                    i += token.size() - 1;
                } else if (i + token.size() >= in.size()) {
                    tmp += in.substr(i, token.size());
                    ret.push_back(tmp);
                    tmp.clear();
                    break;
                } else {
                    tmp += in[i];
                }
            }

            return ret;
        }
    }
}