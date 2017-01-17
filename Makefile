SRCS = timer.c
OBJS = $(SRCS:.c=.o)
TARGET = libtimer.so

CC = mpicc
CFLAGS = -O3 -Wall -fPIC
LDFLAGS = -shared

$LIBS = 

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)
	@echo done.

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -c

clean:
	rm -rf *.so *.o core.* *# *~ $(TARGET)
