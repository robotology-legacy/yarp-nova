
MAIN = nova_client
CODELIB =  libnova.a

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
LIBOBJS = $(patsubst %.cpp, %.o, $(wildcard Nova*.cpp))
LIBINCLUDES = Nova.h $(patsubst %.cpp, %.h, $(wildcard Nova*.cpp))

LIBS = -lYARP_OS -lACE
SUBLIBS = /usr/local/lib/libYARP_OS.a /usr/lib/libACE.a

default: $(MAIN)

%.o: %.cpp
	g++ -DYARP_OS_CONFIG=LINUX -c $< -o $@

$(MAIN): $(OBJS)
	g++ $(OBJS) -o $(MAIN) $(LIBS)

$(CODELIB): $(LIBOBJS)
	@echo Making $(CODELIB) for $(LIBOBJS) $(SUBLIBS)
	$(AR) $(ARFLAGS) $(CODELIB) $(LIBOBJS) $(SUBLIBS)

install: $(CODELIB)
	cp $(CODELIB) ../lib
	cp $(LIBINCLUDES) ../include

clean:
	rm -f $(MAIN) $(OBJS)
