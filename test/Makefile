XSRVLIB=test
CFLAGS=-I../thirdparty/libev/include/
LDFLAGS=-L../thirdparty/libev/lib/
LIBSRCS=test.cc
LIBOBJS=$(addprefix ./, $(addsuffix .o, $(basename $(LIBSRCS))))
all: $(XSRVLIB)
%.o: %.cc
	g++ $(CFLAGS) -o $@ -c $^
$(XSRVLIB):$(LIBOBJS)
	g++ $(LDFLAGS) -o $@ $^ -lev

clean:
	rm $(LIBOBJS) $(XSRVLIB) -f
