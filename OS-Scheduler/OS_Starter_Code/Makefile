build:
	gcc headers.h 
	gcc output_files/output_utils.h 
	gcc Data_Structures/pcb_linked_list.h
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out
	gcc scheduler.c -o scheduler.out
	gcc Data_Structures/priority_queue.h
	gcc output_files/output_utils.h
	gcc Algorithms/SRTN.c -o SRTN.out
	gcc -lm Algorithms/RR.c -o RR.out 
	gcc -lm Algorithms/HPF.c -o HPF.out 
	
	
clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./process_generator.out
