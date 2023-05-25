//
//  main.cpp
//  cpusim
//
//  Created by flutterdro on 06.05.2023.
//
#include "space-invaders.h"




int main() {
    std::unique_ptr<Game> inv = std::make_unique<Game>();
    inv->run();

    glfwTerminate();
    std::cout << "Hello, World!\n";
    return 0;
}
