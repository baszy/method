#pragma once

// TODO: VC++ and MinGW and clang and ...
#if __GNUC__ >= 4
    #define METHOD_DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define METHOD_DLL_PRIVATE  __attribute__ ((visibility ("hidden")))
#else
    #define METHOD_DLL_PUBLIC
    #define METHOD_DLL_PRIVATE
#endif

// TODO: We should use these everywhere
typedef void Void;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float F32;
typedef double F64;
