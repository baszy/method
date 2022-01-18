#pragma once

// Profiling capabilities
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef METHOD_PROFILE
    static inline unsigned long long get_time() {
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return 1000000000ull * now.tv_sec + now.tv_nsec;
    }
#else
    #define get_time() (void)0
#endif
