#include "Defs.hpp"

namespace method {

U16 swap_endian(U16 value) {
    union {
        U16 short_part;
        U8 char_part[4];
    } source, dest;

    source.short_part = value;

    dest.char_part[0] = source.char_part[1];
    dest.char_part[1] = source.char_part[0];

    return dest.short_part;
}

U32 swap_endian(U32 value) {
    union {
        U32 int_part;
        U8 char_part[4];
    } source, dest;

    source.int_part = value;

    dest.char_part[0] = source.char_part[3];
    dest.char_part[1] = source.char_part[2];
    dest.char_part[2] = source.char_part[1];
    dest.char_part[3] = source.char_part[0];

    return dest.int_part;
}

}
