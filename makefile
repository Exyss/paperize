CC := gcc
CFLAGS := -Wall -Wextra

output: compile clean
	$(info )
	$(info +++ Done!)

compile: precompile
	$(info )
	$(info +++ Compiling project...)
	$(CC) $(CFLAGS) -O3 *.o -o paperize

precompile:  main data_structs core

main: main.c args.c
	$(info )
	$(info +++ Pre-compiling main file...)
	gcc -c main.c -o main.o
	gcc -c args.c -o args.o

data_structs: node.c queue.c linked_list.c
	$(info )
	$(info +++ Pre-compiling data structs modules...)
	gcc -c node.c -o node.o
	gcc -c queue.c -o queue.o
	gcc -c linked_list.c -o linked_list.o

core: reader.c pager.c formatter.c core/writer.c structs.c string_utils.c single_process.c multi_process.c
	$(info )
	$(info +++ Pre-compiling core modules...)
	gcc -c reader.c -o reader.o
	gcc -c pager.c -o pager.o
	gcc -c formatter.c -o formatter.o
	gcc -c core/writer.c -o writer.o
	gcc -c structs.c -o structs.o
	gcc -c string_utils.c -o string_utils.o
	gcc -c single_process.c -o single_process.o
	gcc -c multi_process.c -o multi_process.o

clean:
	$(info )
	$(info +++ Deleting generated .o files...)
	rm -f *.o $(PROGS)