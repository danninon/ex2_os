# Makefile for ex2_q1 winter 2023A

CFLAGS   = -Wall -std=c99
LDFLAGS  = -lm
CC       = gcc
ECHO     = @echo "going to build target $@ (dependent on $^)"
PROG = q1_ex2
PROGS =  $(PROG) hello_who hello 

all: $(PROGS) test

hello: hello.c
	$(ECHO)
	$(CC) $^ $(CFLAGS) -o $@

hello_who: hello_who.c
	$(ECHO)
	$(CC) $^ $(CFLAGS) -o $@

$(PROG): $(PROG).c
	$(ECHO)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -vf *.o ${PROGS} t1.txt

test: $(PROG)
	@echo going to run test...
	# please DO NOT erase the above two lines. 
	# However you may add your tests here 
	# don't forget the lines should start with <tab> and not spaces.
	./q1_ex2 "1>" t1.txt hello
	cat t1.txt
	./q1_ex2 "2>" t2.txt hello
	cat t2.txt
	./q1_ex2 "2>" t2.txt hello_who 
	cat t2.txt


  
  
