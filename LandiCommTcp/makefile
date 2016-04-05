CCO=gcc
VPATH=
INCLUDES=
LIBS=-lz  -lm  -lnsl -ldl
LIBPATH=/lib/
COMMONPATH=$(VPATH)
SONAME=liblanditms.so
LIBNAME=liblanditms.so.1
LIBFILENAME=liblanditms.so.1.0
LIBCMD= -shared -lc -W1,-soname,$(LIBNAME)

clean:
	rm -rf *.o

$(SONAME): Commu.o LandiCommTcp.o ListenPosThread.o ListenServerThread.o MyLog.o
	gcc $(LIBCMD) -o $(LIBFILENAME) Commu.o LandiCommTcp.o ListenPosThread.o ListenServerThread.o MyLog.o
	ln -sf $(LIBFILENAME) $(SONAME)
	ln -sf $(LIBFILENAME) $(LIBNAME)
	
%.o: %.c %.h
	$(CCO) -c $< -o $@
#	$(CCO) -c $(INCLUDES) $< -o $(COMMON)$@


