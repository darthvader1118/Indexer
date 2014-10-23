COMPILER = gcc
CCFLAGS = -Wall -g

all: index

indexer.o: indexer.c indexer.h 
		$(COMPILER) $(CCFLAGS) -c indexer.c 
		
tokenizer.o: tokenizer.c tokenizer.h
		$(COMPILER) $(CCFLAGS) -c tokenizer.c
		
index: indexer.o tokenizer.o
		$(COMPILER) $(CCFLAGS) -o index indexer.o tokenizer.o

error:
	gcc -g -Wall indexer indexer.c
clean:
		rm -f *.o
		rm -f index
		rm -f *.h.gch
