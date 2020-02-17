OBJS	= main.o utils.o cmdline.o
SOURCE	= main.c utils.c cmdline.c
HEADER	= utils.h cmdline.h
OUT	= bitaplong.out
CC	 = gcc
FLAGS	 = -g -c -Wall -Wextra -O2 
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

utils.o: utils.c
	$(CC) $(FLAGS) utils.c 

cmdline.o: cmdline.c
	$(CC) $(FLAGS) cmdline.c 


clean:
	rm -f $(OBJS) $(OUT)
