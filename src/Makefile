
MAIN = nova_client
CODELIB =  libnova.a

YARP_ROOT=/usr/local/src/LIRA/yarp
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
LIBOBJS = $(patsubst %.cpp, %.o, $(wildcard Nova*.cpp))
LIBINCLUDES = Nova.h $(patsubst %.cpp, %.h, $(wildcard Nova*.cpp))
INC = -I $(YARP_ROOT)/include/ -I $(YARP_ROOT)/include/linux/ -L../lib/linux/

LIBS = -lYARP_OS -lACE -lpthread -ldl
SUBLIBS = 

default: $(MAIN)

%.o: %.cpp
	g++32 $(INC) -DYARP_OS_CONFIG=LINUX -c $< -o $@

$(MAIN): $(OBJS)
	g++32 $(INC) $(OBJS) -o $(MAIN) $(LIBS)

$(CODELIB): $(LIBOBJS)
	@echo Making $(CODELIB) for $(LIBOBJS) $(SUBLIBS)
#	$(AR) -rvs $(CODELIB) $(LIBOBJS) $(SUBLIBS)
#	mergelib $(CODELIB) /usr/local/lib/libYARP_OS.a ""
#	mergelib $(CODELIB) /usr/lib/libACE.a ""
#	mergelib $(CODELIB) /usr/lib/libpthread.a ""
#	mergelib $(CODELIB) /usr/lib/libdl.a ""
#	ld $(INC) -shared -o $(CODELIB) $(LIBOBJS) $(LIBS)
	ar -sr $(CODELIB) $(LIBOBJS)


install: $(CODELIB)
	mkdir -p ../lib
	mkdir -p ../lib/linux
	mkdir -p ../include
	cp $(CODELIB) ../lib/linux
	cp $(LIBINCLUDES) ../include

clean:
	rm -f $(MAIN) $(OBJS) $(CODELIB)
