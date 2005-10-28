
MAIN = nova_client

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

LIBDIR = ../lib/linux

CFLAGS = -L$(LIBDIR) -I../include -g

LIBS = -lnova -lACE -lncurses

default: $(MAIN)


# library is in a non-standard place
run:
	export LD_LIBRARY_PATH=$(LIBDIR) && ./$(MAIN) muse

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

$(MAIN): $(OBJS)
	g++ $(CFLAGS) $(OBJS) -o $(MAIN) $(LIBS)

clean:
	rm -f $(MAIN) $(OBJS)

