CC := $(CC)
CFLAGS := -Wall -Wextra

output: compile clean
	$(info )
	$(info +++ Done!)

compile: precompile
	$(info )
	$(info +++ Compiling project...)
	$(CC) $(CFLAGS) -O3 *.o -o paperize

precompile: main.c
	$(info )
	$(info +++ Pre-compiling main file...)
	$(CC) -c main.c -o main.o

	$(info )
	$(info +++ Pre-compiling utils/ modules...)
	$(CC) -c utils/args.c -o args.o
	$(CC) -c utils/string_utils.c -o string_utils.o

	$(info )
	$(info +++ Pre-compiling data_struct/ modules...)
	$(CC) -c data-structs/node.c -o node.o
	$(CC) -c data-structs/queue.c -o queue.o
	$(CC) -c data-structs/linked_list.c -o linked_list.o

	$(info )
	$(info +++ Pre-compiling core modules...)
	$(CC) -c core/reader.c -o reader.o
	$(CC) -c core/pager.c -o pager.o
	$(CC) -c core/formatter.c -o formatter.o
	$(CC) -c core/writer.c -o writer.o
	$(CC) -c core/structs.c -o structs.o

	$(info )
	$(info +++ Pre-compiling spark/ modules...)
	$(CC) -c spark/multi_process.c -o multi_process.o
	$(CC) -c spark/single_process.c -o single_process.o

clean:
	$(info )
	$(info +++ Removing compilation files...)
	rm -f *.o $(PROGS)