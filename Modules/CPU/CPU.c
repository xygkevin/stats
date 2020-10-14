//
//  CPU.c
//  CPU
//
//  Created by Serhiy Mytrovtsiy on 13/10/2020.
//  Using Swift 5.0.
//  Running on macOS 10.15.
//
//  Copyright © 2020 Serhiy Mytrovtsiy. All rights reserved.
//

#include "CPU.h"

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

#if __has_include(<IntelPowerGadget/PowerGadgetLib.h>)
#include <IntelPowerGadget/PowerGadgetLib.h>

pthread_t thread_id;
double CPUFrequency;
bool stop;
double min;
double max;
PGSampleID sampleID_1;
PGSampleID sampleID_2;

void *listener () {
    while(stop == false) {
        PG_ReadSample(0, &sampleID_1);
        sleep(1);
        PG_ReadSample(0, &sampleID_2);

        PGSample_GetIAFrequency(sampleID_1, sampleID_2, &CPUFrequency, &min, &max);

        PGSample_Release(sampleID_2);
        PGSample_Release(sampleID_1);
    }

    pthread_exit(NULL);
}

void PG_start() {
    stop = false;
    PG_Initialize();
    pthread_create(&thread_id, NULL, listener, NULL);
}

void PG_stop() {
    stop = true;
    PG_Shutdown();
}

double* PG_getCPUFrequency() {
    if (stop == true || CPUFrequency == 0) {
        return NULL;
    } else {
        return &CPUFrequency;
    }
    return NULL;
}
#else
void PG_start() {}
void PG_stop() {}

double* PG_getCPUFrequency() {
    return NULL;
}
#endif
