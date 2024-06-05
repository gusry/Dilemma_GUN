CC= x86_64-w64-mingw32-gcc
CFLAGS= -Wall -g
SRCS=main.c
TARGET=Dilemma_GUN 
OBJS := $(patsubst %.c,%.o,$(SRCS))

all : $(TARGET) 
%.o:%.c
	$(CC) $(CCFLAGS) $< -c -o $@
$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@


.PHONY=clean

clean:
	rm -f $(OBJS) $(TARGET) 
