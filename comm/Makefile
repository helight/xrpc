XSRVLIB=comm.o
CFLAGS=-I../include/ -I../include/comm/ -I../include/xsrv/ -m32
LIBSRCS=config.cc  dbconn.cc
LIBOBJS=$(addprefix ./, $(addsuffix .o, $(basename $(LIBSRCS))))
all: $(XSRVLIB)
%.o: %.cc
	g++ $(CFLAGS) -o $@ -c $^
$(XSRVLIB):$(LIBOBJS)
	ar -rcs $@ $^

clean:
	rm $(LIBOBJS) $(XSRVLIB) -f
