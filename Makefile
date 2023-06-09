CC := $(CC)

CFLAGS := -Wall -Wextra

SRC_DIR := src

output: compile clean
	$(info )
	$(info +++ Done!)

compile: precompile
	$(info )
	$(info +++ Compiling project...)
	$(CC) $(CFLAGS) -O3 *.o -o paperize

precompile: $(SRC_DIR)/main.c
	$(info )
	$(info +++ Pre-compiling main file...)
	$(CC) -c $(SRC_DIR)/main.c -o main.o

	$(info )
	$(info +++ Pre-compiling utils/ modules...)
	$(CC) -c $(SRC_DIR)/utils/args.c -o args.o
	$(CC) -c $(SRC_DIR)/utils/string_utils.c -o string_utils.o

	$(info )
	$(info +++ Pre-compiling data_struct/ modules...)
	$(CC) -c $(SRC_DIR)/data_structs/node.c -o node.o
	$(CC) -c $(SRC_DIR)/data_structs/queue.c -o queue.o
	$(CC) -c $(SRC_DIR)/data_structs/linked_list.c -o linked_list.o

	$(info )
	$(info +++ Pre-compiling core modules...)
	$(CC) -c $(SRC_DIR)/core/reader.c -o reader.o
	$(CC) -c $(SRC_DIR)/core/pager.c -o pager.o
	$(CC) -c $(SRC_DIR)/core/formatter.c -o formatter.o
	$(CC) -c $(SRC_DIR)/core/writer.c -o writer.o
	$(CC) -c $(SRC_DIR)/core/page_structs.c -o page_structs.o

	$(info )
	$(info +++ Pre-compiling spark/ modules...)
	$(CC) -c $(SRC_DIR)/spark/multi_process.c -o multi_process.o
	$(CC) -c $(SRC_DIR)/spark/single_process.c -o single_process.o

clean:
	$(info )
	$(info +++ Removing compilation files...)
	@rm -f *.o