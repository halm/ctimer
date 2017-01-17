SRCS = example_mpi.c
OBJS = $(SRCS:.c=.o)
TARGET = example_mpi
CC = mpicc
CCFLAGS = -O3 -Wall 
LDFLAGS = 
LIBS = -L../ -ltimer
INC = -I../

all: $(OBJS)
	$(CC) $(INC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

.c.o:
	$(CC) $(INC) $(CCFLAGS) $(@:.o=.c) -c

clean:
	rm -rf *.o core.* *# *~ $(TARGET)
	echo $(OBJS)
