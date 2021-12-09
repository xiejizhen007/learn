#include <iostream>
#include <vector>
#include "scene.hpp"
#include "OBJLoader.hpp"
#include <sstream>

int main() {
    Scene* scene = Scene::getInstance();
    scene->initScene();
    scene->drawScene();
}