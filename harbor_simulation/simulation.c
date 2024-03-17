/*
Estimate the expected utilization of each berth and of the cranes. 
*/

#include "simulation.h"
#include "ship_queue.h"
#include<assert.h>

extern FILE *exponential_RV_File, *uniform_RV_File;

double current_time;
double total_delay;
int  num_delays;
double max_delay, min_delay;

int n;
int m;
double avg_delays[MAX_SHIPS][MAX_SHIPS];
Berth berths[2];

double expon_RV(){
	double expRV;
	fscanf(exponential_RV_File, "%lf", &expRV);
	return expRV;
}
double uni_RV(){
	double uniRV;
	fscanf(uniform_RV_File, "%lf", &uniRV);
	return uniRV;
}
int getQuickerBusyBerth(){
    // if both berths are busy, return the one that will finish sooner
    if(berths[0].is_busy && berths[1].is_busy){
        if( berths[0].end_time <  berths[1].end_time)
            return 0;
        else 
            return 1;
	}
    else{
        if(berths[0].is_busy )return 0;
        else if(berths[1].is_busy)return 1;
        else return -1;
    }
}
int getEmptyBerth(){
	if(!berths[0].is_busy)return 0;
	else if(!berths[1].is_busy)return 1;
	else return -1;
}
void initialize(void){
    current_time = 0.0;
	total_delay=0.0;
	num_delays=0;
	max_delay=0;
	min_delay=1000;

	clearShipQueue();

    for(int i=0;i<2;i++){
		berths[i].is_busy=false;
		berths[i].is_full_service=false;
		berths[i].queue_wait=0;
		berths[i].start_time=0;
    	berths[i].end_time=0;
		berths[i].total_busy_time=0;
	}

}
// An arrival event occured
void arrive(){
	//printf("Arrival event at time: %lf\n",current_time);
	int i=getEmptyBerth();

	if(i==-1) //both berths busy
		shipEnqueue(current_time);
	else 
		enterBerth(i,0);
}
// Enter Berth event
void enterBerth(int i, double queue_wait){
		//printf("enter berth(%d) event at time: %lf\n",i,current_time);
		int j = !i; //the other berth is j

		//the ship will enter berth i
		double service_time = uni_RV();

		//go to  berth i 
		berths[i].is_busy=true;
		berths[i].queue_wait = queue_wait;
		berths[i].start_time=current_time;
		
		if(!berths[j].is_busy){ //the other berth is empty, use 2 cranes
			berths[i].end_time=current_time + service_time/2;
			berths[i].is_full_service = true;
		}

		else { // the other berth is busy
			berths[i].end_time=current_time + service_time;
			berths[i].is_full_service = false;
			//printf("will double time for berth %lf %lf\n",berths[j].start_time, berths[j].end_time-berths[j].start_time);

			if(berths[j].is_full_service){
				berths[j].is_full_service=false;
				double rem = berths[j].end_time-current_time;
				berths[j].end_time = current_time + rem*2;
			}

		}	
}
// Leave Berth event
void leaveBerth(int i){ 
	//printf("leave berth (%d) event at time: %lf\n",i,current_time);
	int j = !i; //the other berth is j

	assert(berths[i].is_busy);

	//leave  berth i 
	berths[i].is_busy=false;
	
	berths[i].total_busy_time+=(berths[i].end_time - berths[i].start_time);
	double delay = (berths[i].end_time - berths[i].start_time) + berths[i].queue_wait;
	min_delay = fmin(min_delay, delay);
	max_delay = fmax(max_delay, delay);

	total_delay += delay;
	num_delays ++;

	avg_delays[n][m] = total_delay / num_delays;
	m++; 

	if(isEmptyShipQueue()){ // half time of berth j if busy
		if(berths[j].is_busy && !berths[j].is_full_service){ 
			berths[j].is_full_service=true;
			double rem = berths[j].end_time-current_time;
			berths[j].end_time = current_time + rem/2;
		}
	}
	else {
		double ship_arrival = shipDequeue();
		enterBerth(i, current_time-ship_arrival);
	}

}
