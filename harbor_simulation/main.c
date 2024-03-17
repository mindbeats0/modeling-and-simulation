#include <stdio.h>
#include"run.h"
#include"statistics.h"

FILE *exponential_RV_File, *uniform_RV_File, *statistics_FILE;
int main() {
    exponential_RV_File = fopen("random_numbers/exp.in", "r");
    uniform_RV_File = fopen("random_numbers/uni.in", "r"); 
    statistics_FILE = fopen("../output/statistics.out", "w"); 

    // we will make 60 replications, each of size 300
    int runs_num=60;
    int runs_length=300;

    printf("\nSTART TRANSIENT STATE SIMULATION\n");
    printf("--------------------------------\n");

    /*
        Run the simulation Assuming that no ships are in the harbor at time 0
        Till reaching the steady state
    */
    for(int i=0;i<runs_num;i++)
        run(i,runs_length,0);
    

    start_matlab();
    computeAverageDelays();
    computeMovingAverage(3);
    computeMovingAverage(7);
    computeMovingAverage(15);
    plot(0);
    end_matlab();

   
    printf("\nSTART STEADY STATE SIMULATION\n");
    printf("-----------------------------\n");

    /*
        After reaching the steady state
        Run the simulation for 90 days
    */
    for(int i=0;i<runs_num;i++)
        run(i,runs_length,1);
    
    start_matlab();
    computeAverageDelays();
    computeMovingAverage(3);
    computeMovingAverage(7);
    computeMovingAverage(15);
    plot(1);
    end_matlab();
 
    print_statistics();

    fclose(exponential_RV_File); 
    fclose(uniform_RV_File);
    fclose(statistics_FILE);
    return 0;
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
