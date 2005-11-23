
MAIN = nova_server

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

YARP_HOME = /usr/local/src/LIRA/yarp
LIBDIR = /usr/local/src/LIRA/nova/lib/linux
INCDIR = -I$(YARP_HOME)/include -I../include -I.

CFLAGS = $(INCDIR) -g


#LIBS = -lnova -lACE -lpthread -lYARP_OS -ldl
LIBS = -lnova -lYARP_OS -lACE -lpthread -ldl



CC = g++32

default: $(MAIN)


# library is in a non-standard place
run:
	export LD_LIBRARY_PATH=$(LIBDIR) && ./$(MAIN)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN) $(LIBS)

clean:
	rm -f $(MAIN) $(OBJS)

