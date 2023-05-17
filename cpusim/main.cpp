//
//  main.cpp
//  cpusim
//
//  Created by flutterdro on 06.05.2023.
//

#include <fmt/core.h>
#include <iostream>
#include "cpu.h"
//#include "alu.h"
//#include "cu.h"
//#include "cpu.h"
//#include "registers.h"
//#include "dissasemble.h"


int main() {
//    std::string hexf;
//    Dissasembler diss;
//    diss.dissasemble("assets/invaders/invaders");
//    hexf = diss.getResult();
//
//    fmt::print(hexf + '\n');
    CPU intel8080;
    intel8080.loadMemory("assets/invaders/invaders");
    while(true) {
        std::cin.get();
        fmt::print("{:x}: instruction: {:#x}", intel8080.getPc(), intel8080.getInstr());
        intel8080.printContent();
        intel8080.executeNextInstruction();
    }
    
    return 0;
}
