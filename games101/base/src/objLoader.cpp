#include "objLoader.hpp"
#include <iostream>
#include <sstream>      // stream
#include <fstream>      // file I/O
#include <vector>
#include <string>

namespace obj {
    namespace algorithm {
        std::string firstToken(const std::string& line) {
            size_t start = line.find_first_not_of(" ");
            size_t end = line.find_first_of(" ");

            return line.substr(start, end - start);
        }

        std::string tailToken(const std::string& line) {
            size_t token_start = line.find_first_not_of(" ");
            size_t space_start = line.find_first_of(" ", token_start);
            size_t tail_start = line.find_first_not_of(" ", space_start);
            size_t tail_end = line.find_last_not_of(" ");
            return line.substr(tail_start, tail_end - tail_start + 1);
        }

        std::vector<std::string> split(std::string in, const std::string& token) {
            std::vector<std::string> out;
            std::string tmp;

            for (int i = 0; i < in.size(); i++) {
                if (in[i] != ' ') {
                    tmp += in[i];
                    if (tmp == token) {
                        out.push_back(tmp);
                        tmp.clear();
                    } else if (i == in.size() - 1) {
                        out.push_back(tmp);
                        tmp.clear();
                    }
                } else {
                    if (tmp.size()) {
                        out.push_back(tmp);
                        tmp.clear();
                    }
                }
            }

            return out;
        }

        float str2number(const std::string& str) {
            std::istringstream istr(str);
            float ret;

            istr >> ret;
            return ret;
        }
    }

    bool
    Loader::loadFile(std::string path) {
        // 先检查是不是 obj 文件格式
        if (path.substr(path.size() - 4, 4) != ".obj") {
            std::cout << "error > path <" << std::endl;
            return false;
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cout << "error > path <" << std::endl;
            return false;
        }

        // 避免二次加载导致模型错乱
        loadedMeshs.clear();
        loadedIndices.clear();
        loadedVertices.clear();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::vector<Vector3> positions;
        std::vector<Vector3> normals;
        std::vector<Vector2> texCoords;

        std::string line;
        while (std::getline(file, line)) {
            if (algorithm::firstToken(line) == "o") {
                std::cout 
                    << ">> object name: " 
                    << algorithm::tailToken(line)
                    << std::endl;
            } 
            // 顶点
            else if (algorithm::firstToken(line) == "v") {
                auto vpos = algorithm::split(line, "v");
                Vector3 v;
                v.x = std::stof(vpos[1]);
                v.y = std::stof(vpos[2]);
                v.z = std::stof(vpos[3]);

                positions.push_back(v);
            } 
            // 法线
            else if (algorithm::firstToken(line) == "vn") {

            } 
            // 纹理坐标
            else if (algorithm::firstToken(line) == "vn") {

            }
        }

        std::cout << ">> vertices: " << positions.size() << std::endl;
    }

    bool 
    Loader::loadObjectMtl(std::string path) {

    }
}