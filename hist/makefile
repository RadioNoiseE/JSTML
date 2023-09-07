.PONY : complie
.SUFFIXES : .o
.c.o :
	clang -o ${.TARGET} -c ${.IMPSRC} 

complie : asrtktp.o creattok.o jstml.o jstml.h jstparse.o scantok.o textokenize.o tokbuf.o
	clang tokbuf.o creattok.o scantok.o asrtktp.o jstparse.o textokenize.o jstml.o -o jstml.wasm 
