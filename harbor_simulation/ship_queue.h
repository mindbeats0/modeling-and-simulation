#ifndef SHIP_QUEUE_H
#define SHIP_QUEUE_H

#define MAX_SHIPS 10000

void clearShipQueue();
double shipQueueFront();
bool isEmptyShipQueue();
int shipQueueSize();
void shipEnqueue(double ship);
double shipDequeue();

#endif