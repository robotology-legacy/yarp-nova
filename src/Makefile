
MAIN = nova_client
CODELIB =  libnova.so

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
LIBOBJS = $(patsubst %.cpp, %.o, $(wildcard Nova*.cpp))
LIBINCLUDES = Nova.h $(patsubst %.cpp, %.h, $(wildcard Nova*.cpp))

LIBS = -lYARP_OS -lACE
SUBLIBS = 

default: $(MAIN)

%.o: %.cpp
	g++ -DYARP_OS_CONFIG=LINUX -c $< -o $@

$(MAIN): $(OBJS)
	g++ $(OBJS) -o $(MAIN) $(LIBS)

$(CODELIB): $(LIBOBJS)
	@echo Making $(CODELIB) for $(LIBOBJS) $(SUBLIBS)
#	$(AR) -rvs $(CODELIB) $(LIBOBJS) $(SUBLIBS)
#	mergelib $(CODELIB) /usr/local/lib/libYARP_OS.a ""
#	mergelib $(CODELIB) /usr/lib/libACE.a ""
#	mergelib $(CODELIB) /usr/lib/libpthread.a ""
#	mergelib $(CODELIB) /usr/lib/libdl.a ""
	ld -shared -o $(CODELIB) $(LIBOBJS) $(LIBS)


install: $(CODELIB)
	mkdir -p ../lib
	mkdir -p ../lib/linux
	mkdir -p ../include
	cp $(CODELIB) ../lib/linux
	cp $(LIBINCLUDES) ../include

clean:
	rm -f $(MAIN) $(OBJS) $(CODELIB)
