
MAIN = nova_client
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
LIBS = -lYARP_OS -lACE

%.o: %.cpp
	g++ -DYARP_OS_CONFIG=LINUX -c $< -o $@

$(MAIN): $(OBJS)
	g++ $(OBJS) -o $(MAIN) $(LIBS)
