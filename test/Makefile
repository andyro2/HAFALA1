# Makefile for the smash program
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
OBJS = test.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o test $(OBJS)
# Creating the object files
test.o: test.h
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

