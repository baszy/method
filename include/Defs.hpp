#pragma once

#include <cstdint>

// TODO: VC++ and MinGW and clang and ...
#if __GNUC__ >= 4
    #define METHOD_DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define METHOD_DLL_PRIVATE  __attribute__ ((visibility ("hidden")))
#else
    #define METHOD_DLL_PUBLIC
    #define METHOD_DLL_PRIVATE
#endif

namespace method {

// TODO: We should use these everywhere
typedef void Void;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float F32;
typedef double F64;

U16 swap_endian(U16 value);
U32 swap_endian(U32 value);

}
