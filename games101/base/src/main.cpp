#include <iostream>
#include "scene.hpp"

int main() {
    Scene* scene = new Scene();
    scene->initScene();
    scene->drawScene();
}