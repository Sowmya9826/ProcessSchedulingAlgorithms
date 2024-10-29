CC = gcc
FLAGS = -Wall -Wextra

SRC_FILES = common.c process.c FCFS.c HPFNP.c HPFNP_AGING.c HPFP.c HPFP_AGING.c RR.c SJF.c SRT.c main.c
OBJ_FILES = $(SRC_FILES:.c=.o)
OUTPUT = scheduler

.PHONY: build clean execute

build: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES)
	$(CC) $(FLAGS) $(OBJ_FILES) -o $(OUTPUT)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(OUTPUT)

execute: clean build
	./$(OUTPUT) $(RR_TIME_SLICE)

