%: %.c
	$(CC) $(CFLAGS) -o $@ $<

all: $(basename $(wildcard *.c))
	mv simstart /usr/bin
	mv simstop /usr/bin
	mv simulate /usr/bin

clean: 
	rm -rf simstart simstop simulate
