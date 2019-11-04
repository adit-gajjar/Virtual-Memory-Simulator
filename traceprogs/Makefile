SRCS = simpleloop.c matmul.c 
PROGS = simpleloop matmul 

all : $(PROGS)

$(PROGS) : % : %.c
	gcc -Wall -g -o $@ $<


traces: $(PROGS)
	./runit simpleloop
	./runit matmul 100

.PHONY: clean
clean : 
	rm -f simpleloop matmul tr-*.ref *.marker *~
