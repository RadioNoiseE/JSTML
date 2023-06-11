.PHONY: complie
complie: asrtktp.c creattok.c jstml.c jstml.h jstparse.c scantok.c textokenize.c tokbuf.c
	clang tokbuf.c creattok.c scantok.c asrtktp.c jstparse.c textokenize.c jstml.c -o jstml.wasm
