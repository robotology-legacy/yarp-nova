
MAIN = nova_server

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

CFLAGS = -L../lib -I../include

LIBS = -lnova -lACE

default: $(MAIN)


# library is in a non-standard place
run:
	export LD_LIBRARY_PATH=../lib && ./$(MAIN)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

$(MAIN): $(OBJS)
	g++ $(CFLAGS) $(OBJS) -o $(MAIN) $(LIBS)

clean:
	rm -f $(MAIN) $(OBJS)

