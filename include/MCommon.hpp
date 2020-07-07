#pragma once

#if __GNUC__ >= 4
    #define METHOD_API_PUBLIC __attribute__ ((visibility ("default")))
    #define METHOD_API_PRIVATE  __attribute__ ((visibility ("hidden")))
#else
    #define METHOD_API_PUBLIC
    #define METHOD_API_PRIVATE
#endif