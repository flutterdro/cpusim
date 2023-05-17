//
//  cpu.cpp
//  cpusim
//
//  Created by flutterdro on 10.05.2023.
//

#include "cpu.h"

CPU::CPU()
: m_alu(m_registers), m_memory(UINT16_MAX, 0), m_pc(m_memory.begin()) {}

void CPU::loadMemory(const char *filename) {
    if (std::ifstream fin{filename, std::ios::hex }) {
        uint8_t buff;
        for (int i = 0; fin >> std::noskipws >> buff; i++) {
            m_memory[i] = buff;
        }
    }
    else {
        printf("failed to open");
    }
}

void CPU::printContent() {
    fmt::print("accumulator: {:d}, flags: {:b} \n", m_registers.a, static_cast<uint8_t>(m_registers.flags));
}

void CPU::interupt(uint8_t opcode) {
    if (interuptable) executeInstruction(opcode);
}

void CPU::executeNextInstruction() {
    executeInstruction(*m_pc);
}

void CPU::executeInstruction(uint8_t opcode) {
    switch (opcode) {
        case 0x00:  m_pc += 1;
            break;
        //
        ///Mostly imediate instruction as well as register pairs instructions
        //
        case 0x01:  {                                                           //LXI BC, d16
            m_registers.b = *(m_pc + 2);
            m_registers.c = *(m_pc + 1);
            m_pc += 3;
            break;
        }
        case 0x02:  m_memory[m_registers.bc] = m_registers.a;   m_pc += 1;      //STAX BC
            break;
        case 0x03:  m_alu.inx(m_registers.bc);                  m_pc += 1;
            break;
        case 0x04:  m_alu.inr(m_registers.b);                   m_pc += 1;
            break;
        case 0x05:  m_alu.dcr(m_registers.b);                   m_pc += 1;
            break;
        case 0x06:  m_registers.b = *(m_pc + 1);                m_pc += 2;      //MVI B, d8
            break;
        case 0x07:  m_alu.rlc();                                m_pc += 1;
            break;
        case 0x08:  break; // TODO: write a LOG class and here make a warning   //*NOP
        case 0X09:  m_alu.dad(m_registers.bc);                  m_pc += 1;
            break;
        case 0x0a:  m_registers.a = m_memory[m_registers.bc];   m_pc += 1;      //LDAX BC
            break;
        case 0x0b:  m_alu.dcx(m_registers.bc);                  m_pc += 1;
            break;
        case 0x0c:  m_alu.inr(m_registers.c);                   m_pc += 1;
            break;
        case 0x0d:  m_alu.dcr(m_registers.c);                   m_pc += 1;
            break;
        case 0x0e:  m_registers.c = *(m_pc + 1);                m_pc += 2;      //MVI C, d8
            break;
        case 0x0f:  m_alu.rrc();                                m_pc += 1;
            break;
        case 0x10:  break; // TODO: write a LOG class and here make a warning   //*NOP
        case 0x11:  {                                                           //LXI DE, d16
            m_registers.d = *(m_pc + 2);
            m_registers.e = *(m_pc + 1);
            m_pc += 3;
            break;
        }
        case 0x12:  m_memory[m_registers.de] = m_registers.a;   m_pc += 1;      //STAX DE
            break;
        case 0x13:  m_alu.inx(m_registers.de);                  m_pc += 1;
            break;
        case 0x14:  m_alu.inr(m_registers.d);                   m_pc += 1;
            break;
        case 0x15:  m_alu.dcr(m_registers.d);                   m_pc += 1;
            break;
        case 0x16:  m_registers.d = *(m_pc + 1);                m_pc += 2;      //MVI D, d8
            break;
        case 0x17:  m_alu.ral();                                m_pc += 1;
            break;
        case 0x18:  break; // TODO: write a LOG class and here make a warning   //*NOP
        case 0x19:  m_alu.dad(m_registers.de);                  m_pc += 1;
            break;
        case 0x1a:  m_registers.a = m_memory[m_registers.de];   m_pc += 1;      //LDAX DE
            break;
        case 0x1b:  m_alu.dcx(m_registers.de);                  m_pc += 1;
            break;
        case 0x1c:  m_alu.inr(m_registers.e);                   m_pc += 1;
            break;
        case 0x1d:  m_alu.dcr(m_registers.e);                   m_pc += 1;
            break;
        case 0x1e:  m_registers.e = *(m_pc + 1);                m_pc += 2;      //MVI E, d8
            break;
        case 0x1f:  m_alu.rar();                                m_pc += 1;
            break;
        case 0x20:  break; // TODO: write a LOG class and here make a warning   //*NOP
        case 0x21:  {                                                           //LXI HL, d16
            m_registers.h = *(m_pc + 2);
            m_registers.l = *(m_pc + 1);
            m_pc += 3;
            break;
        }
        case 0x22: {                                                            //SHLD a16
            uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_memory[addr] = m_registers.l;
            m_memory[addr + 1] = m_registers.h;
            m_pc += 3;
            break;
        }
        case 0x23:  m_alu.inx(m_registers.hl);                  m_pc += 1;
            break;
        case 0x24:  m_alu.inr(m_registers.h);                   m_pc += 1;
            break;
        case 0x25:  m_alu.dcr(m_registers.h);                   m_pc += 1;
            break;
        case 0x26:  m_registers.h = *(m_pc + 1);                m_pc += 2;      //MVI H, d8
            break;
        case 0x27:  m_alu.daa();                                m_pc += 1;
            break;
        case 0x28:  break; // TODO: write a LOG class and here make a warning   //*NOP
        case 0x29:  m_alu.dad(m_registers.hl);                  m_pc += 1;
            break;
        case 0x2a: {                                                            //LHLD a16
            uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_registers.l = m_memory[addr + 1];
            m_registers.h = m_memory[addr];
            m_pc += 3;
            break;
        }
        case 0x2b:  m_alu.dcx(m_registers.hl);                  m_pc += 1;
            break;
        case 0x2c:  m_alu.inr(m_registers.l);                   m_pc += 1;
            break;
        case 0x2d:  m_alu.dcr(m_registers.l);                   m_pc += 1;
            break;
        case 0x2e:  m_registers.l = *(m_pc + 1);                m_pc += 2;
            break;
        case 0x2f:  m_alu.cma();                                m_pc += 1;      //MVI L, d8
            break;
        case 0x30:  break; // TODO: write a LOG class and here make a warning   //*NOP
        case 0x31: {                                                            //LXI SP, d16
            uint16_t data = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_sp = m_memory.begin() + data;
            m_pc += 3;
            break;
        }
        case 0x32: {                                                            //STA a16
            uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_memory[addr] = m_registers.a;
            m_pc += 3;
            break;
        }
        case 0x33:  m_sp += 1;                                  m_pc += 1;
            break;
        case 0x34:  m_alu.inr(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0x35:  m_alu.dcr(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0x36:  m_memory[m_registers.hl] = *(m_pc + 1);     m_pc += 2;      //MVI M, d8
            break;
        case 0x37:  m_alu.stc();                                m_pc += 1;
            break;
        case 0x38:  break; //TODO: write a LOG class and here make a warning//   *NOP
        case 0x39:  m_alu.dad(m_sp - m_memory.begin());         m_pc += 1;
            break;
        case 0x3a:  {
            uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_registers.a = m_memory[addr];
            m_pc += 3;
            break;
        }
        case 0x3b:  m_sp -= 1;                                  m_pc += 1;
            break;
        case 0x3c:  m_alu.inr(m_registers.a);                   m_pc += 1;
            break;
        case 0x3d:  m_alu.dcr(m_registers.a);                   m_pc += 1;
            break;
        case 0x3e:  m_registers.a = *(m_pc + 1);                m_pc += 2;      //MVI A, d8
            break;
        case 0x3f:  m_alu.cmc();                                m_pc += 1;
            break;
        //
        ///MOV instructions start here
        //
        case 0x40:  m_registers.b = m_registers.b;              m_pc += 1;
            break;
        case 0x41:  m_registers.b = m_registers.c;              m_pc += 1;
            break;
        case 0x42:  m_registers.b = m_registers.d;              m_pc += 1;
            break;
        case 0x43:  m_registers.b = m_registers.e;              m_pc += 1;
            break;
        case 0x44:  m_registers.b = m_registers.h;              m_pc += 1;
            break;
        case 0x45:  m_registers.b = m_registers.l;              m_pc += 1;
            break;
        case 0x46:  m_registers.b = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x47:  m_registers.b = m_registers.a;              m_pc += 1;
            break;
        case 0x48:  m_registers.c = m_registers.b;              m_pc += 1;
            break;
        case 0x49:  m_registers.c = m_registers.c;              m_pc += 1;
            break;
        case 0x4a:  m_registers.c = m_registers.d;              m_pc += 1;
            break;
        case 0x4b:  m_registers.c = m_registers.e;              m_pc += 1;
            break;
        case 0x4c:  m_registers.c = m_registers.h;              m_pc += 1;
            break;
        case 0x4d:  m_registers.c = m_registers.l;              m_pc += 1;
            break;
        case 0x4e:  m_registers.c = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x4f:  m_registers.c = m_registers.a;              m_pc += 1;
            break;
        case 0x50:  m_registers.d = m_registers.b;              m_pc += 1;
            break;
        case 0x51:  m_registers.d = m_registers.c;              m_pc += 1;
            break;
        case 0x52:  m_registers.d = m_registers.d;              m_pc += 1;
            break;
        case 0x53:  m_registers.d = m_registers.e;              m_pc += 1;
            break;
        case 0x54:  m_registers.d = m_registers.h;              m_pc += 1;
            break;
        case 0x55:  m_registers.d = m_registers.l;              m_pc += 1;
            break;
        case 0x56:  m_registers.d = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x57:  m_registers.d = m_registers.a;              m_pc += 1;
            break;
        case 0x58:  m_registers.e = m_registers.b;              m_pc += 1;
            break;
        case 0x59:  m_registers.e = m_registers.c;              m_pc += 1;
            break;
        case 0x5a:  m_registers.e = m_registers.d;              m_pc += 1;
            break;
        case 0x5b:  m_registers.e = m_registers.e;              m_pc += 1;
            break;
        case 0x5c:  m_registers.e = m_registers.h;              m_pc += 1;
            break;
        case 0x5d:  m_registers.e = m_registers.l;              m_pc += 1;
            break;
        case 0x5e:  m_registers.e = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x5f:  m_registers.e = m_registers.a;              m_pc += 1;
            break;
        case 0x60:  m_registers.h = m_registers.b;              m_pc += 1;
            break;
        case 0x61:  m_registers.h = m_registers.c;              m_pc += 1;
            break;
        case 0x62:  m_registers.h = m_registers.d;              m_pc += 1;
            break;
        case 0x63:  m_registers.h = m_registers.e;              m_pc += 1;
            break;
        case 0x64:  m_registers.h = m_registers.h;              m_pc += 1;
            break;
        case 0x65:  m_registers.h = m_registers.l;              m_pc += 1;
            break;
        case 0x66:  m_registers.h = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x67:  m_registers.h = m_registers.a;              m_pc += 1;
            break;
        case 0x68:  m_registers.l = m_registers.b;              m_pc += 1;
            break;
        case 0x69:  m_registers.l = m_registers.c;              m_pc += 1;
            break;
        case 0x6a:  m_registers.l = m_registers.d;              m_pc += 1;
            break;
        case 0x6b:  m_registers.l = m_registers.e;              m_pc += 1;
            break;
        case 0x6c:  m_registers.l = m_registers.h;              m_pc += 1;
            break;
        case 0x6d:  m_registers.l = m_registers.l;              m_pc += 1;
            break;
        case 0x6e:  m_registers.l = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x6f:  m_registers.l = m_registers.a;              m_pc += 1;
            break;
        case 0x70:  m_memory[m_registers.hl] = m_registers.b;   m_pc += 1;
            break;
        case 0x71:  m_memory[m_registers.hl] = m_registers.c;   m_pc += 1;
            break;
        case 0x72:  m_memory[m_registers.hl] = m_registers.d;   m_pc += 1;
            break;
        case 0x73:  m_memory[m_registers.hl] = m_registers.e;   m_pc += 1;
            break;
        case 0x74:  m_memory[m_registers.hl] = m_registers.h;   m_pc += 1;
            break;
        case 0x75:  m_memory[m_registers.hl] = m_registers.l;   m_pc += 1;
            break;
        case 0x76:  halted = 1;                                 m_pc += 1;      //HLT <- reeeeeally~ important instruction
            break;
        case 0x77:  m_memory[m_registers.hl] = m_registers.a;   m_pc += 1;
            break;
        case 0x78:  m_registers.a = m_registers.b;              m_pc += 1;
            break;
        case 0x79:  m_registers.a = m_registers.c;              m_pc += 1;
            break;
        case 0x7a:  m_registers.a = m_registers.d;              m_pc += 1;
            break;
        case 0x7b:  m_registers.a = m_registers.e;              m_pc += 1;
            break;
        case 0x7c:  m_registers.a = m_registers.h;              m_pc += 1;
            break;
        case 0x7d:  m_registers.a = m_registers.l;              m_pc += 1;
            break;
        case 0x7e:  m_registers.a = m_memory[m_registers.hl];   m_pc += 1;
            break;
        case 0x7f:  m_registers.a = m_registers.a;              m_pc += 1;
            break;
        //
        ///Arithmethic and logic group of instructions
        //
        case 0x80:  m_alu.add(m_registers.b);                   m_pc += 1;
            break;
        case 0x81:  m_alu.add(m_registers.c);                   m_pc += 1;
            break;
        case 0x82:  m_alu.add(m_registers.d);                   m_pc += 1;
            break;
        case 0x83:  m_alu.add(m_registers.e);                   m_pc += 1;
            break;
        case 0x84:  m_alu.add(m_registers.h);                   m_pc += 1;
            break;
        case 0x85:  m_alu.add(m_registers.l);                   m_pc += 1;
            break;
        case 0x86:  m_alu.add(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0x87:  m_alu.add(m_registers.a);                   m_pc += 1;
            break;
        case 0x88:  m_alu.adc(m_registers.b);                   m_pc += 1;
            break;
        case 0x89:  m_alu.adc(m_registers.c);                   m_pc += 1;
            break;
        case 0x8a:  m_alu.adc(m_registers.d);                   m_pc += 1;
            break;
        case 0x8b:  m_alu.adc(m_registers.e);                   m_pc += 1;
            break;
        case 0x8c:  m_alu.adc(m_registers.h);                   m_pc += 1;
            break;
        case 0x8d:  m_alu.adc(m_registers.l);                   m_pc += 1;
            break;
        case 0x8e:  m_alu.adc(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0x8f:  m_alu.adc(m_registers.a);                   m_pc += 1;
            break;
        case 0x90:  m_alu.sub(m_registers.b);                   m_pc += 1;
            break;
        case 0x91:  m_alu.sub(m_registers.c);                   m_pc += 1;
            break;
        case 0x92:  m_alu.sub(m_registers.d);                   m_pc += 1;
            break;
        case 0x93:  m_alu.sub(m_registers.e);                   m_pc += 1;
            break;
        case 0x94:  m_alu.sub(m_registers.h);                   m_pc += 1;
            break;
        case 0x95:  m_alu.sub(m_registers.l);                   m_pc += 1;
            break;
        case 0x96:  m_alu.sub(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0x97:  m_alu.sub(m_registers.a);                   m_pc += 1;
            break;
        case 0x98:  m_alu.sbb(m_registers.b);                   m_pc += 1;
            break;
        case 0x99:  m_alu.sbb(m_registers.c);                   m_pc += 1;
            break;
        case 0x9a:  m_alu.sbb(m_registers.d);                   m_pc += 1;
            break;
        case 0x9b:  m_alu.sbb(m_registers.e);                   m_pc += 1;
            break;
        case 0x9c:  m_alu.sbb(m_registers.h);                   m_pc += 1;
            break;
        case 0x9d:  m_alu.sbb(m_registers.l);                   m_pc += 1;
            break;
        case 0x9e:  m_alu.sbb(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0x9f:  m_alu.sbb(m_registers.a);                   m_pc += 1;
            break;
        case 0xa0:  m_alu.ana(m_registers.b);                   m_pc += 1;
            break;
        case 0xa1:  m_alu.ana(m_registers.c);                   m_pc += 1;
            break;
        case 0xa2:  m_alu.ana(m_registers.d);                   m_pc += 1;
            break;
        case 0xa3:  m_alu.ana(m_registers.e);                   m_pc += 1;
            break;
        case 0xa4:  m_alu.ana(m_registers.h);                   m_pc += 1;
            break;
        case 0xa5:  m_alu.ana(m_registers.l);                   m_pc += 1;
            break;
        case 0xa6:  m_alu.ana(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0xa7:  m_alu.ana(m_registers.a);                   m_pc += 1;
            break;
        case 0xa8:  m_alu.xra(m_registers.b);                   m_pc += 1;
            break;
        case 0xa9:  m_alu.xra(m_registers.c);                   m_pc += 1;
            break;
        case 0xaa:  m_alu.xra(m_registers.d);                   m_pc += 1;
            break;
        case 0xab:  m_alu.xra(m_registers.e);                   m_pc += 1;
            break;
        case 0xac:  m_alu.xra(m_registers.h);                   m_pc += 1;
            break;
        case 0xad:  m_alu.xra(m_registers.l);                   m_pc += 1;
            break;
        case 0xae:  m_alu.xra(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0xaf:  m_alu.xra(m_registers.a);                   m_pc += 1;
            break;
        case 0xb0:  m_alu.ora(m_registers.b);                   m_pc += 1;
            break;
        case 0xb1:  m_alu.ora(m_registers.c);                   m_pc += 1;
            break;
        case 0xb2:  m_alu.ora(m_registers.d);                   m_pc += 1;
            break;
        case 0xb3:  m_alu.ora(m_registers.e);                   m_pc += 1;
            break;
        case 0xb4:  m_alu.ora(m_registers.h);                   m_pc += 1;
            break;
        case 0xb5:  m_alu.ora(m_registers.l);                   m_pc += 1;
            break;
        case 0xb6:  m_alu.ora(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0xb7:  m_alu.cmp(m_registers.a);                   m_pc += 1;
            break;
        case 0xb8:  m_alu.cmp(m_registers.b);                   m_pc += 1;
            break;
        case 0xb9:  m_alu.cmp(m_registers.c);                   m_pc += 1;
            break;
        case 0xba:  m_alu.cmp(m_registers.d);                   m_pc += 1;
            break;
        case 0xbb:  m_alu.cmp(m_registers.e);                   m_pc += 1;
            break;
        case 0xbc:  m_alu.cmp(m_registers.h);                   m_pc += 1;
            break;
        case 0xbd:  m_alu.cmp(m_registers.l);                   m_pc += 1;
            break;
        case 0xbe:  m_alu.cmp(m_memory[m_registers.hl]);        m_pc += 1;
            break;
        case 0xbf:  m_alu.cmp(m_registers.a);                   m_pc += 1;
            break;
        //
        ///Branching instructions and some immediate arithmetics
        //
        case 0xc0:  {                                                           //RNZ
            if (!(m_registers.flags & FlagRegister::Zero)) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xc1:  {                                                           //POP BC
            m_registers.c = *m_sp;
            m_sp += 1;
            m_registers.b = *m_sp;
            m_sp += 1;
            m_pc += 1;
            break;
        }
        case 0xc2:  {                                                           //JNZ a16
            if (!(m_registers.flags & FlagRegister::Zero)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xc3:  {                                                           //JMP a16
            uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_pc = m_memory.begin() + addr;
            break;
        }
        case 0xc4:   {                                                          //CNZ a16
            if (!(m_registers.flags & FlagRegister::Zero)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            }  else {
                m_pc += 3;
            }
            break;
        }
        case 0xc5:  {                                                           //PUSH BC
            m_sp -= 1;
            *m_sp = m_registers.b;
            m_sp -= 1;
            *m_sp = m_registers.c;
            m_pc += 1;
            break;
        }
        case 0xc6:  m_alu.add(*(m_pc + 1));                      m_pc += 2;
            break;
        case 0xc7:  {                                                           //RST 0
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin();
            break;
        }
        case 0xc8:  {                                                           //RZ
            if (m_registers.flags & FlagRegister::Zero) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xc9: {                                                            //RET
            m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
            m_sp += 2;
            break;
        }
        case 0xca: {                                                            //JZ a16
            if (m_registers.flags & FlagRegister::Zero) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xcb:  break; // TODO: write a LOG class and here make a warning   //*JMP a16
        case 0xcc:  {                                                           //CZ a16
            if (m_registers.flags & FlagRegister::Zero) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xcd:  {                                                           //CALL a16
            uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc  = m_memory.begin() + addr;
            break;
        }
        case 0xce:  m_alu.adc(*(m_pc + 1));                     m_pc += 2;
            break;
        case 0xcf:  {                                                           //RST 1
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08 * 0x01;
            break;
        }
        case 0xd0:  {                                                           //RNC
            if (!(m_registers.flags & FlagRegister::Carry)) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xd1:  {                                                           //POP DE
            m_registers.e = *m_sp;
            m_sp += 1;
            m_registers.d = *m_sp;
            m_sp += 1;
            m_pc += 1;
            break;
        }
        case 0xd2:  {                                                           //JNC a16
            if (!(m_registers.flags & FlagRegister::Carry)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        //I'm actually not sure how it works
        case 0xd3:  m_out_port[*(m_pc + 1)] = m_registers.a;        m_pc += 2;  //OUT d8
            break;
        case 0xd4:   {                                                          //CNC a16
            if (!(m_registers.flags & FlagRegister::Carry)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xd5:  {                                                           //PUSH DE
            m_sp -= 1;
            *m_sp = m_registers.d;
            m_sp -= 1;
            *m_sp = m_registers.e;
            m_pc += 1;
            break;
        }
        case 0xd6:  m_alu.sub(*(m_pc + 1));                      m_pc += 2;
            break;
        case 0xd7:  {                                                           //RST 2
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08*0x02;
            break;
        }
        case 0xd8:  {                                                           //RC
            if (m_registers.flags & FlagRegister::Carry) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xd9:  break; // TODO: write a LOG class and here make a warning   //*RET
        case 0xda: {                                                            //JC a16
            if (m_registers.flags & FlagRegister::Carry) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xdb:  m_registers.a = m_in_port[*(m_pc + 1)];     m_pc += 2;      //IN d8
            break;
        case 0xdc:  {                                                           //CC a16
            if (m_registers.flags & FlagRegister::Carry) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xdd:  break; // TODO: write a LOG class and here make a warning   //*CALL a16
        case 0xde:  m_alu.sbb(*(m_pc + 1));                     m_pc += 2;
            break;
        case 0xdf:  {                                                           //RST 3
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08 * 0x03;
            break;
        }
        case 0xe0:  {                                                           //RPO
            if (!(m_registers.flags & FlagRegister::Parity)) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xe1:  {                                                           //POP HL
            m_registers.l = *m_sp;
            m_sp += 1;
            m_registers.h = *m_sp;
            m_sp += 1;
            m_pc += 1;
            break;
        }
        case 0xe2:  {                                                           //JPO a16
            if (!(m_registers.flags & FlagRegister::Parity)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xe3:  {                                                           //XTHL
            std::swap(m_registers.l, *m_sp);
            std::swap(m_registers.h, *(m_sp + 1));
            m_pc += 1;
            break;
        }
        case 0xe4:   {                                                          //CPO a16
            if (!(m_registers.flags & FlagRegister::Parity)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xe5:  {                                                           //PUSH HL
            m_sp -= 1;
            *m_sp = m_registers.h;
            m_sp -= 1;
            *m_sp = m_registers.l;
            m_pc += 1;
            break;
        }
        case 0xe6:  m_alu.ana(*(m_pc + 1));                      m_pc += 2;
            break;
        case 0xe7:  {                                                           //RST 4
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08*0x04;
            break;
        }
        case 0xe8:  {                                                           //RPE
            if (m_registers.flags & FlagRegister::Parity) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xe9:  m_pc = m_memory.begin() + m_registers.hl;                   //*RET
            break;
        case 0xea: {                                                            //JPE a16
            if (m_registers.flags & FlagRegister::Parity) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xeb:  std::swap(m_registers.hl, m_registers.de);  m_pc += 1;      //XCHG
            break;
        case 0xec:  {                                                           //CPE a16
            if (m_registers.flags & FlagRegister::Parity) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xed:  break; // TODO: write a LOG class and here make a warning   //*CALL a16
        case 0xee:  m_alu.xra(*(m_pc + 1));                     m_pc += 2;
            break;
        case 0xef:  {                                                           //RST 3
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08 * 0x05;
            break;
        }
        case 0xf0:  {                                                           //RP
            if (!(m_registers.flags & FlagRegister::Sign)) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xf1:  {                                                           //POP PSW
            m_registers.flags = static_cast<FlagRegister>(*m_sp);
            m_sp += 1;
            m_registers.a = *m_sp;
            m_sp += 1;
            m_pc += 1;
            break;
        }
        case 0xf2:  {                                                           //JP a16
            if (!(m_registers.flags & FlagRegister::Sign)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xf3:  interuptable = 0;                           m_pc += 1;      //DI
            break;
        case 0xf4:   {                                                          //CP a16
            if (!(m_registers.flags & FlagRegister::Sign)) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xf5:  {                                                           //PUSH PSW
            m_sp -= 1;
            *m_sp = m_registers.a;
            m_sp -= 1;
            *m_sp = static_cast<uint8_t>(m_registers.flags);
            m_pc += 1;
            break;
        }
        case 0xf6:  m_alu.ora(*(m_pc + 1));                      m_pc += 2;
            break;
        case 0xf7:  {                                                           //RST 6
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08*0x06;
            break;
        }
        case 0xf8:  {                                                           //RM
            if (m_registers.flags & FlagRegister::Sign) {
                m_pc = m_memory.begin() + concatenate(*(m_sp + 1), *m_sp);
                m_sp += 2;
            } else {
                m_pc += 1;
            }
            break;
        }
        case 0xf9:  m_sp = m_memory.begin() + m_registers.hl;                   //SPHL
            break;
        case 0xfa: {                                                            //JM a16
            if (m_registers.flags & FlagRegister::Sign) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_pc = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xfb:  interuptable = 1;                           m_pc += 1;      //EI
            break;
        case 0xfc:  {                                                           //CM a16
            if (m_registers.flags & FlagRegister::Sign) {
                uint16_t addr = concatenate(*(m_pc + 2), *(m_pc + 1));
                m_sp -= 1;
                *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
                m_sp -= 1;
                *m_sp = m_pc + 3 - m_memory.begin();
                m_pc  = m_memory.begin() + addr;
            } else {
                m_pc += 3;
            }
            break;
        }
        case 0xfd:  break; // TODO: write a LOG class and here make a warning   //*CALL a16
        case 0xfe:  m_alu.cmp(*(m_pc + 1));                     m_pc += 2;
            break;
        case 0xff:  {                                                           //RST 7
            m_sp -= 1;
            *m_sp = (m_pc + 3 - m_memory.begin()) >> 4;
            m_sp -= 1;
            *m_sp = m_pc + 3 - m_memory.begin();
            m_pc = m_memory.begin() + 0x08 * 0x07;
            break;
        }
    }
}
