
#ifndef SIMULATION_H
#define SIMULATION_H

//C code for the external definitions, queueing model.
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef struct Berth{
    bool is_busy;
    bool is_full_service;
    double queue_wait;
    double start_time;
    double end_time;
    double total_busy_time;
}Berth;
double expon_RV();
double uni_RV();
int getQuickerBusyBerth();
int getEmptyBerth();
void initialize(void);
void arrive();
void enterBerth(int i, double queue_wait);
void leaveBerth(int i);

#endif