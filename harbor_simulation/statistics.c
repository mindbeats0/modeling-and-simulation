
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "ship_queue.h"
#include "statistics.h"
#include "simulation.h"


extern int n;
extern int m;

extern double avg_delays[MAX_SHIPS][MAX_SHIPS];
extern FILE *statistics_FILE;

double Y_BAR[MAX_SHIPS];
double mov_avg[20][MAX_SHIPS];

extern double max_delay, min_delay;
extern double current_time;
extern Berth berths[2];
Engine *ep;
mxArray *D, *E;

double events[MAX_SHIPS];
void start_matlab(){
	D= NULL;
	E = NULL;
	if (!(ep = engOpen(""))) {
		fprintf(stderr, "\nCan't start MATLAB engine\n");
	}
	for(int i=0;i<m;i++)events[i]=(double)i;
	E = mxCreateDoubleMatrix(1, m, mxREAL);
	memcpy((void *)mxGetPr(E), (void *)events, m*sizeof(double));
	engPutVariable(ep, "E", E);

	D = mxCreateDoubleMatrix(1, m, mxREAL);
}
void end_matlab(){
	mxDestroyArray(D);
	mxDestroyArray(E);
	engClose(ep);
}

//plot delay for each replication
void plotRawDelays(){
	for(int rep=0;rep<n;rep++){
		
		memcpy((void *)mxGetPr(D), (void *)avg_delays[rep], m*sizeof(double));
		engPutVariable(ep, "D", D);

				
		engEvalString(ep, "plot(E,D);");
		//engEvalString(ep, "ylim([0.5 1.2]);");
		engEvalString(ep, "title('delays');");
		engEvalString(ep, "xlabel('Time (days)');");
		engEvalString(ep, "ylabel('Avg Delays (days)');");


		printf("\nPlot for replication %d\nPress y to continue...\n",rep);
		printf(">>\n");
		char s[2];
		scanf("%s",s);

	}
	// int BUFSIZE = 256;
	// char buffer[BUFSIZE+1];
	// buffer[BUFSIZE] = '\0';
	// engOutputBuffer(ep, buffer, BUFSIZE);
	// engEvalString(ep, "E");
	// printf("%s", buffer);// Echo the output from the command.  
	// engEvalString(ep, "close;");

}

void computeAverageDelays(){
	for(int event=0;event<m;event++){
		Y_BAR[event]=0;
		for(int rep=0;rep<n;rep++){
			Y_BAR[event]+=avg_delays[rep][event];
		}
		Y_BAR[event]/=n;
	}
}

//w<=m/4
//m=60, w<=15
void computeMovingAverage(int w){

	mov_avg[w][0] = Y_BAR[0];

	for(int i=1;i<m-w;i++){
		double sum=0;
		if(i>=w){
			for(int j=i-w; j<=i+w ;j++){
				assert(j<m);
				sum+=Y_BAR[j];
			}
			sum/=2*w+1;
		} else {
			int new_win=i;
			for(int j=i-new_win; j<=i+new_win ;j++){
				assert(j<m);
				sum+=Y_BAR[j];
			}
			sum/=2*new_win+1;
		}
		mov_avg[w][i] = sum; 
	}
}

void print_statistics(){
	fprintf(statistics_FILE, "Minimum Delay: %lf days\n",min_delay);
	fprintf(statistics_FILE, "Maximum Delay: %lf days\n",max_delay);
	fprintf(statistics_FILE, "Average Delay: %lf days\n",Y_BAR[m]);
	fprintf(statistics_FILE, "Expected Utilization of first berth and crane: %lf days\n",berths[0].total_busy_time/current_time);
	fprintf(statistics_FILE, "Expected Utilization of second berth and crane: %lf days\n",berths[1].total_busy_time/current_time);
}
void plot(int is_steady_state){

	char str[60];
	engEvalString(ep, "t = tiledlayout(2,2);");	

	//Plot Average Delay

	memcpy((void *)mxGetPr(D), (void *)Y_BAR, m*sizeof(double));
	engPutVariable(ep, "D", D);

	engEvalString(ep, "nexttile;");	
	engEvalString(ep, "plot(E,D);");
	if(is_steady_state)engEvalString(ep, "ylim([0.7 0.8]);");
	engEvalString(ep, "title('average delays');");
	engEvalString(ep, "xlabel('Time (days)');");
	engEvalString(ep, "ylabel('Avg Delays (days)');");



	//plotMovingAverages
	for(int i=0;i<3;i++){
		int w;
		if(i==0)w=3;
		else if(i==1)w=7;
		else w=15;

		memcpy((void *)mxGetPr(E), (void *)events, (m-w)*sizeof(double));
		engPutVariable(ep, "E", E);

		memcpy((void *)mxGetPr(D), (void *)mov_avg[w], (m-w)*sizeof(double));
		engPutVariable(ep, "D", D);


		engEvalString(ep, "nexttile;");	
		engEvalString(ep, "plot(E,D);");
		if(is_steady_state)engEvalString(ep, "ylim([0.7 0.8]);");
		sprintf(str, "title('Moving average delays with window size: %d');",w);
		engEvalString(ep, str);
		engEvalString(ep, "xlabel('Time (days)');");
		engEvalString(ep, "ylabel('Avg Delays (days)');");
	}

	// Requires R2020a or later
	if(is_steady_state)
		engEvalString(ep,"exportgraphics(t,'../output/steady_state_plots.eps')");
	else 
		engEvalString(ep,"exportgraphics(t,'../output/transient_state_plots.eps')");

	printf("Plots for Average and Moving Averages\nPress y to continue...\n");
	printf(">> ");
	char s[2];
	scanf("%s",s);
}