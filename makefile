CC := gcc
CFLAGS := -Wall -Wextra

output: compile clean
	$(info )
	$(info +++ Done!)

compile: precompile
	$(info )
	$(info +++ Compiling project...)
	$(CC) $(CFLAGS) -O3 *.o -o paperize

precompile: structs data_structs io_modules  main

main: main.c
	$(info )
	$(info +++ Pre-compiling main file...)
	gcc -c main.c -o main.o

data_structs: node.c queue.c linked_list.c
	$(info )
	$(info +++ Pre-compiling data structures...)
	gcc -c node.c -o node.o
	gcc -c queue.c -o queue.o
	gcc -c linked_list.c -o linked_list.o

io_modules: file_reader.c file_writer.c
	$(info )
	$(info +++ Pre-compiling I/O modules...)
	gcc -c file_reader.c -o file_reader.o
	gcc -c file_writer.c -o file_writer.o

structs: structs.c
	$(info +++ Pre-compiling program structs...)
	gcc -c structs.c -o structs.o

clean:
	$(info )
	$(info +++ Deleting generated .o files...)
	rm -f *.o $(PROGS)