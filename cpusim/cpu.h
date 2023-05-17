//
//  cpu.h
//  cpusim
//
//  Created by flutterdro on 10.05.2023.
//

#ifndef cpu_h
#define cpu_h

#include <vector>
#include <fmt/core.h>
#include <array>
#include <bitset>
#include <iterator>

#include "alu.h"
#include <fstream>
#include "registers.h"


class CPU {
public:
    CPU();
    void executeInstruction(uint8_t opcode);
    void executeNextInstruction();
    void interupt(uint8_t opcode);
    void loadMemory(const char* filename);
    uint16_t getPc() {return std::distance(m_memory.begin(), m_pc);}
    void printContent();
    uint8_t getInstr() {return *m_pc;}
private:
    Registers m_registers;
    ArithmeticLogicUnit m_alu;
    std::vector<uint8_t> m_memory;
    std::array<uint8_t, 8> m_in_port;
    std::array<uint8_t, 8> m_out_port;
    //THERE WAS NO NEED TO MAKE THEM ITERATORS
    //WHY ARE YOU SO STUPID
    //IT ONLY COMPLICATED THINGS
    //WRYYYYYYYYY
    //it not only makes it harder but even more inacurate as the way to emulate sp and pc
    std::vector<uint8_t>::iterator m_sp;  //stack pointer
    std::vector<uint8_t>::iterator m_pc;  //program counter
    
    uint8_t halted : 1;
    uint8_t interuptable : 1;
private:
    uint16_t concatenate(uint8_t high, uint8_t low) {return low | (high << 8);}
};

#endif /* cpu_h */
