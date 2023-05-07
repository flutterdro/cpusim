#include "alu.h"

FlagRegister operator|(FlagRegister lhs, FlagRegister rhs) {
    return static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) |
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}

FlagRegister operator&(FlagRegister lhs, FlagRegister rhs) {
    return static_cast<FlagRegister>(
        static_cast<std::underlying_type_t<FlagRegister>>(lhs) &
        static_cast<std::underlying_type_t<FlagRegister>>(rhs)
    );
}
