#include <stdio.h>
#include <assert.h>
#include"run.h"
#include"simulation.h"
#include"ship_queue.h"

extern double current_time;
extern Berth berths[2];
extern double total_delay;
extern int num_delays;
extern int n;
extern int m;
void run(int run_num, int run_length, int steady_state) {

    if(!steady_state)initialize();
    m=0;
    n=run_num;
    double new_arrival = current_time + expon_RV();

    double start_time=current_time;
    while(m<run_length){
        if(steady_state && current_time - start_time >=90)break;
        //Check if there is a departure event OR an Arrival event
        int i = getQuickerBusyBerth();
        if(i!=-1 &&  berths[i].end_time < new_arrival){   
            current_time = berths[i].end_time;
            leaveBerth(i);
        } 
        else {
            current_time = new_arrival;
            new_arrival = current_time + expon_RV();
            arrive();
        }

    }
}



/*
    Single-server queueing system 
    Mean interarrival time  1.000 minutes 
    Mean service time       0.500 minutes
    Number of customers     1000

    Average delay in queue  0.430 minutes 
    Average number in queue  0.418
    Server utilization       0.460
    Time simulation ended   1027.915 minutes
*/
