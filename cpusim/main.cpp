//
//  main.cpp
//  cpusim
//
//  Created by flutterdro on 06.05.2023.
//

#include <fmt/core.h>
#include "alu.h"
#include "cu.h"
#include "registers.h"
#include "dissasemble.h"

int main() {
    std::string hexf;
    Dissasembler diss;
    diss.dissasemble("assets/invaders/invaders.h");
    hexf = diss.getResult();
           
    fmt::print(hexf + '\n');
    return 0;
}
