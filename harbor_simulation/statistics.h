#ifndef STATISTICS_H
#define STATISTICS_H

void start_matlab();
void end_matlab();

void plotRawDelays();
void computeAverageDelays();
void computeMovingAverage(int w);
void plot(int is_steady_state);
void print_statistics();
#endif
//void update_time_avg_stats(void); 