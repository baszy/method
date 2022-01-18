#include "Profiler.hpp"

#include <iostream>

using namespace std;

namespace {

// TODO: No mutex
const int FPS_RB_SIZE = 10;
static float fps_hist[FPS_RB_SIZE] = {0};

bool running = false;

void * PtFpsCtr(void * ns) {
    timespec delay = {0, 500000000ull};

    while (running) {
        float average = 0;
        for (int i = 0; i < FPS_RB_SIZE; i++)
            average += fps_hist[i];
        average /= FPS_RB_SIZE;
        printf("[Profiler][Performance] fps (%d-frame average): %3.f\n", FPS_RB_SIZE, average);
        nanosleep(&delay, nullptr);
    }

    cout << "[Profiler][Performance] Terminating profiler thread" << endl;
    pthread_exit(0);
}

void create_thread() {
    pthread_t thread;

    unsigned long long ns = 500000000ull;
    int rc = pthread_create(&thread, nullptr, PtFpsCtr, &ns);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
}

void end_thread(pthread_t thread) {
    int * status = new int;
    pthread_join(thread, (void **)&status);
}

};
