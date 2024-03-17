//Queue implementation in C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include"ship_queue.h"

double shipsArray[MAX_SHIPS];
int front, rear, shipsCount;

void clearShipQueue(){
   front = 0;
   rear = -1;
   shipsCount = 0;
}

double shipQueueFront() { return shipsArray[front];}

bool isEmptyShipQueue() { return shipsCount == 0;}

bool isFullShipQueue() { return shipsCount == MAX_SHIPS; }

int shipQueueSize() { return shipsCount;}  

void shipEnqueue(double ship) {

   if(isFullShipQueue()) {
      fprintf(stderr, "\nShip queue if FULL\n");
      exit(2);
   }
      
   if(rear == MAX_SHIPS-1)
      rear = -1;                 

   shipsArray[++rear] = ship;
   shipsCount++;
}

double shipDequeue() {
   double ship = shipsArray[front++];
	
   if(front == MAX_SHIPS) {
      front = 0;
   }
	
   shipsCount--;
   return ship;  
}