GCC = gcc
BIN = cryptprog

all:
	$(GCC) cryptprog.c -o $(BIN) -lpthread

clean:
	rm -f $(BIN) cryptprog.o

update:
	cp $(BIN) ../../../install_folder/
	make clean

