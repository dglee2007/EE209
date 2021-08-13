CC=gcc209
CCFLAGS=-g
TARGET=ish
OBJS=ish.o dfa.o dynarray.o

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(TARGET)

ish.o : ish.c dfa.h dynarray.h
	$(CC) $(CCFLAGS) -c $<

dfa.o : dfa.c dynarray.h
	$(CC) $(CCFLAGS) -c $<

dynarray.o : dynarray.c dynarray.h
	$(CC) $(CCFLAGS) -c $<
all : ish
clobber: clean
	rm -f *~ \#*\# core
clean:
	rm *.o ish
	rm *.o dfa
	rm *.o dynarray

