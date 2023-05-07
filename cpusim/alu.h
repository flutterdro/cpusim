#ifndef alu_h
#define alu_h

#include <type_traits>

enum class FlagRegister : uint8_t {
    Zero            = 0b00000001,
    Carry           = 0b00000010,
    Sign            = 0b00000100,
    Parity          = 0b00001000,
    AuxilaryCarry   = 0b00010000
};

FlagRegister operator&(FlagRegister lhs, FlagRegister rhs);
FlagRegister operator|(FlagRegister lhs, FlagRegister rhs);

class ArithmeticLogicUnit {
public:

private:
    uint8_t m_accumulator;
    uint8_t m_tmpAccumulator;
    uint8_t m_tmpRegister;
    FlagRegister m_flagRegister;
};

#endif
