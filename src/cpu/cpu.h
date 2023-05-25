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
#include <functional>

#include "alu.h"
#include <fstream>
#include <iostream>
#include "registers.h"


class CPU {
public:
    CPU();
    
    void executeNextInstruction();
    void interupt(uint8_t opcode);
    
    void loadMemory(const char* filename);
    
    void connectOutPort(int port_id, std::function<void(uint8_t)> device_action);
    void connectInPort(int port_id, std::function<uint8_t(void)> device_action);
    
    std::vector<uint8_t>::iterator getMemoryIt(uint16_t address);
    uint16_t getPc() {return std::distance(m_memory.begin(), m_pc);}
    uint8_t getInstr() {return *m_pc;}
    
    std::vector<uint8_t> m_memory;
    std::vector<uint8_t>::iterator m_sp;  //stack pointer
    std::vector<uint8_t>::iterator m_pc;  //program counter
private:
    uint8_t interuptable : 1;
    Registers m_registers;
    ArithmeticLogicUnit m_alu;
    std::array<std::function<void(uint8_t)>, 8> m_out_port;
    std::array<std::function<uint8_t(void)>, 8> m_in_port;
    //THERE WAS NO NEED TO MAKE THEM ITERATORS
    //WHY ARE YOU SO STUPID
    //IT ONLY COMPLICATED THINGS
    //WRYYYYYYYYY
    //it not only makes it harder but even more inacurate as the way to emulate sp and pc
    
    uint8_t halted : 1;
private:
    void executeInstruction(uint8_t opcode);
    void memoryWrite(uint16_t addr, uint8_t data);
    uint16_t concatenate(uint8_t high, uint8_t low) {return low | (high << 8);}
    void dummyFunco(uint8_t a);
    uint8_t dummyFunci();
};

#endif /* cpu_h */
