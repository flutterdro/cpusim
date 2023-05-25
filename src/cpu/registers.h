#ifndef registers_h
#define registers_h

#include <type_traits>
#include <cinttypes>

//Soooooo to be completely accurate there was no specific register for flags in 8080 just seperate flags
//but it is just more convinient for me to keep them bundled with other registers
//since I need pass them into the alu regardless

enum class FlagRegister : uint8_t {
    Reset           = 0b00000000,
    Zero            = 0b00000001,
    Carry           = 0b10000000,
    Sign            = 0b00000100,
    Parity          = 0b00001000,
    AuxilaryCarry   = 0b00010000
};

std::underlying_type_t<FlagRegister> operator&(FlagRegister lhs, FlagRegister rhs);
FlagRegister operator|(FlagRegister lhs, FlagRegister rhs);
FlagRegister operator^(FlagRegister lhs, FlagRegister rhs);
FlagRegister operator&=(FlagRegister& lhs, FlagRegister rhs);
FlagRegister operator|=(FlagRegister& lhs, FlagRegister rhs);
FlagRegister operator^=(FlagRegister& lhs, FlagRegister rhs);
FlagRegister operator~(const FlagRegister lhs);

class Registers {
public:
    
    Registers();
    //oh forgive me lord
    uint8_t a;
    FlagRegister flags;
    union {
        uint16_t bc;
        struct {
            uint8_t c;
            uint8_t b;
        };
    };
    union {
        uint16_t de;
        struct {
            uint8_t e;
            uint8_t d;
        };
    };
    union {
        uint16_t hl;
        struct {
            uint8_t l;
            uint8_t h;
        };
    };
};

#endif

