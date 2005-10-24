
MAIN = producer_consumer

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

CFLAGS = -L../lib/linux -I../include

LIBS = -lnova -lYARP_OS -lACE -lpthread -ldl

default: $(MAIN)


# library is in a non-standard place
run:
	export LD_LIBRARY_PATH=../lib/linux && ./$(MAIN)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

$(MAIN): $(OBJS)
	g++ $(CFLAGS) $(OBJS) -o $(MAIN) $(LIBS)

clean:
	rm -f $(MAIN) $(OBJS)

