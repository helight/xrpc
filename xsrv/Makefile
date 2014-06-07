XSRVLIB=xsrv.o
CFLAGS=-I../thirdparty/libev/include/ -I../thirdparty/jsonrpc/
LIBSRCS=xtcpserver.cc commnet.cc parse_conf.cc log.cc xtcpclient.cc xudpserver.cc xudpclient.cc
LIBOBJS=$(addprefix ./, $(addsuffix .o, $(basename $(LIBSRCS))))
all: $(XSRVLIB)
%.o: %.cc
	g++ $(CFLAGS) -o $@ -c $^
$(XSRVLIB):$(LIBOBJS)
	ar -rcs $@ $^

clean:
	rm $(LIBOBJS) $(XSRVLIB) -f
