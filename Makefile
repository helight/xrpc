CFLAGS=-I./include  -I./include/xsrv/ -I./include/comm/ -L./lib -lev -ljsonrpc -Wno-write-strings
all:
	@make -C jsonrpc
	@make -C xsrv
	@make -C comm
	g++ tserver.cc comm/comm.o xsrv/xsrv.o -o tserver $(CFLAGS)
	g++ userver.cc comm/comm.o xsrv/xsrv.o -o userver $(CFLAGS)
	g++ tclient.cc xsrv/xsrv.o -o tclient $(CFLAGS)
	g++ uclient.cc xsrv/xsrv.o -o uclient $(CFLAGS)
clean:
	@make -C jsonrpc clean
	@make -C xsrv clean
	@make -C comm clean
	rm tclient tserver uclient userver -f
