#include "alu.h"

ArithmeticLogicUnit::ArithmeticLogicUnit(Registers &registers)
: m_registers(registers) {}

void ArithmeticLogicUnit::add(uint8_t operand) {
    uint16_t result = operand + m_registers.a;
    //I hate flags I HATE THEM
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result > 0xff) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00 || result == 0x100) {
        m_registers.flags |= FlagRegister::Zero;
    }
    m_registers.a = result;
}

void ArithmeticLogicUnit::adc(uint8_t operand) {
    uint16_t result;
    if (m_registers.flags & FlagRegister::Carry) {
        result = operand + m_registers.a + 1;
    } else {
        result = operand + m_registers.a;
    }
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result > 0xff) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00 || result == 0x100) {
        m_registers.flags |= FlagRegister::Zero;
    }
    m_registers.a = result;
}

void ArithmeticLogicUnit::sub(uint8_t operand) {
    operand = ~operand + 1;
    uint16_t result = m_registers.a + operand;
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result <= 0xff) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00 || result == 0x100) {
        m_registers.flags |= FlagRegister::Zero;
    }
    m_registers.a = result;
}

void ArithmeticLogicUnit::sbb(uint8_t operand) {
    uint16_t result;
    if (m_registers.flags & FlagRegister::Carry) {
        operand = ~(operand + 1) + 1;
        result = operand + m_registers.a;
    } else {
        operand = ~operand + 1;
        result = operand + m_registers.a;
    }
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result <= 0xff) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00 || result == 0x100) {
        m_registers.flags |= FlagRegister::Zero;
    }
    m_registers.a = result;
}

void ArithmeticLogicUnit::ana(uint8_t operand) {
    m_registers.a &= operand;
    m_registers.flags &= FlagRegister::Reset;
    if (m_registers.a & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if (checkParity(m_registers.a)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (m_registers.a == 0x00) {
        m_registers.flags |= FlagRegister::Zero;
    }
}

void ArithmeticLogicUnit::xra(uint8_t operand) {
    m_registers.a ^= operand;
    m_registers.flags &= FlagRegister::Reset;
    if (m_registers.a & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if (checkParity(m_registers.a)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (m_registers.a == 0x00) {
        m_registers.flags |= FlagRegister::Zero;
    }
}

void ArithmeticLogicUnit::ora(uint8_t operand) {
    m_registers.a |= operand;
    m_registers.flags &= FlagRegister::Reset;
    if (m_registers.a & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if (checkParity(m_registers.a)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (m_registers.a == 0x00) {
        m_registers.flags |= FlagRegister::Zero;
    }
}

void ArithmeticLogicUnit::cmp(uint8_t operand) {
    operand = ~operand + 1;
    uint16_t result = m_registers.a + operand;
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result <= 0xff) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00 || result == 0x100) {
        m_registers.flags |= FlagRegister::Zero;
    }
}

void ArithmeticLogicUnit::rlc() {
//    fmt::print("rlc\n");
//    fmt::print("before: {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
    //I actually am a shitcoder in fact
    //uint8_t tmp = (m_registers.flags & FlagRegister::Carry) >> 7;
    m_registers.flags &= ~FlagRegister::Carry;
    m_registers.flags |= static_cast<FlagRegister>(m_registers.a & 0b10000000);
    m_registers.a = m_registers.a << 1 | m_registers.a >> 7;
    //m_registers.a |= tmp;
//    fmt::print("after:  {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
}

void ArithmeticLogicUnit::rrc() {
//    fmt::print("rrc\n");
//    fmt::print("before: {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
    m_registers.flags &= ~FlagRegister::Carry;
    m_registers.flags |= static_cast<FlagRegister>((m_registers.a & 0b00000001) << 7);
    m_registers.a = m_registers.a >> 1 | m_registers.a << 7;
//    fmt::print("after:  {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
}

void ArithmeticLogicUnit::ral() {
//    fmt::print("ral\n");
//    fmt::print("before: {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
    uint8_t tmp = (m_registers.flags & FlagRegister::Carry);
    m_registers.flags &= ~FlagRegister::Carry;
    m_registers.flags |= static_cast<FlagRegister>(m_registers.a & 0b10000000);
    m_registers.a = m_registers.a << 1;
    m_registers.a |= tmp >> 7;
//    fmt::print("after:  {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
}

void ArithmeticLogicUnit::rar() {
//    fmt::print("rar\n");
//    fmt::print("before: {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
    uint8_t tmp = (m_registers.flags & FlagRegister::Carry);
    m_registers.flags &= ~FlagRegister::Carry;
    m_registers.flags |= static_cast<FlagRegister>((m_registers.a & 0b00000001) << 7);
    m_registers.a = m_registers.a >> 1;
    m_registers.a |= tmp;
//    fmt::print("after:  {:d} {:08b}\n", (m_registers.flags & FlagRegister::Carry) >> 7, m_registers.a);
}

void ArithmeticLogicUnit::dad(uint16_t operand) {
    uint32_t result = operand + m_registers.hl;
    m_registers.flags &= ~FlagRegister::Carry;
    if (result > 0xffff) {
        m_registers.flags |= FlagRegister::Carry;
    }
    m_registers.hl = result;
}

void ArithmeticLogicUnit::inr(uint8_t &operand) {
    uint8_t result = operand + 1;
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result < operand) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00) {
        m_registers.flags |= FlagRegister::Zero;
    }
    operand = result;
}

void ArithmeticLogicUnit::dcr(uint8_t &operand) {
    uint8_t result = operand - 1;
    m_registers.flags &= FlagRegister::Reset;
    if (result & 0x80) {
        m_registers.flags |= FlagRegister::Sign;
    }
    if ((operand ^ m_registers.a ^ result) & 0x10) {
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (checkParity(result)) {
        m_registers.flags |= FlagRegister::Parity;
    }
    if (result > operand) {
        m_registers.flags |= FlagRegister::Carry;
    }
    if (result == 0x00) {
        m_registers.flags |= FlagRegister::Zero;
    }
    operand = result;
}

void ArithmeticLogicUnit::cma() {
    m_registers.a = ~m_registers.a;
}

void ArithmeticLogicUnit::daa() {
    if (m_registers.flags & FlagRegister::AuxilaryCarry) {
        m_registers.a += 0x06;
    }
    m_registers.flags &= ~FlagRegister::AuxilaryCarry;
    m_registers.flags &= ~FlagRegister::Carry;
    if ((m_registers.a & 0b00001111) > 0x09) {
        m_registers.a += 0x06;
        m_registers.flags |= FlagRegister::AuxilaryCarry;
    }
    if (((m_registers.a & 0b11110000) >> 4) > 0x09) {
        m_registers.a += 0x60;
        m_registers.flags |= FlagRegister::Carry;
    }
}

void ArithmeticLogicUnit::inx(uint16_t &operand) {
    operand++;
}

void ArithmeticLogicUnit::dcx(uint16_t &operand) {
    operand--;
}

void ArithmeticLogicUnit::cmc() {
    m_registers.flags ^= FlagRegister::Carry;
}

void ArithmeticLogicUnit::stc() {
    m_registers.flags |= FlagRegister::Carry;
}

bool ArithmeticLogicUnit::checkParity(uint8_t byte) {
    bool res = 0;
    for (int i = 0; i < 8; i++) {
        res ^= (byte >> i)&0x01;
    }
    return !res;
}

//FlagRegister ArithmeticLogicUnit::setFlagOnCondition(FlagRegister flag, bool condition) {
//    return static_cast<FlagRegister>(
//                                     condition *
//                                     static_cast<std::underlying_type_t<FlagRegister>>(flag)
//                                     );
//}
