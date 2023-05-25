#ifndef alu_h
#define alu_h

#include <type_traits>
#include <cinttypes>
#include <fmt/format.h>
#include "registers.h"

class ArithmeticLogicUnit {
public:
    ArithmeticLogicUnit(Registers &registers);
    void add (uint8_t operand);
    void adc (uint8_t operand);
    void sub (uint8_t operand);
    void sbb (uint8_t operand);
    void dad (uint16_t operand);
    void ana (uint8_t operand);
    void xra (uint8_t operand);
    void ora (uint8_t operand);
    void cmp (uint8_t operand);
    void cma ();
    void daa ();
    void inr (uint8_t &operand);
    void dcr (uint8_t &operand);
    void inx (uint16_t &operand);
    void dcx (uint16_t &operand);
    void rlc ();
    void rrc ();
    void ral ();
    void rar ();
    void cmc ();
    void stc ();
private:
    Registers& m_registers;
private:
    FlagRegister setFlagOnCondition(FlagRegister flag, bool condition);
    bool checkParity(uint8_t byte);
};

#endif
