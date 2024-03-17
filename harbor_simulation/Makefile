
all: main.o run.o ship_queue.o simulation.o statistics.o
	 gcc -g main.o run.o ship_queue.o simulation.o statistics.o -o simulationApp \
	 -L/Applications/MATLAB_R2022b.app/bin/maci64 -leng -lmat -lmx  -Wl,-rpath,/Applications/MATLAB_R2022b.app/bin/maci64 
	 rm -f *.o

#Clean 
.PHONY: clean
clean: 
	rm -f *.o
	rm -f simulationApp 
	
#---------------------------------------------------------------
#build all object files

main.o: main.c run.h 
	gcc -c -g main.c 

run.o: run.c  run.h simulation.h 
	gcc -c -g run.c 


ship_queue.o: ship_queue.c  ship_queue.h
	gcc -c -g ship_queue.c 

simulation.o: simulation.c  simulation.h ship_queue.h
	gcc -c -g simulation.c 

statistics.o: statistics.c  statistics.h ship_queue.h simulation.h
	gcc -c -g statistics.c -Imatlab/include  



