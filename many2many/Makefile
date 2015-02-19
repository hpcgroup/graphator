CC	= CC

all: multi_a2a 

multi_a2a: multi_a2a.C
	$(CC) -O3 -o multi_a2a $< 

trace: multi_a2a.C
	$(CC) -o multi_a2a.trace $< $(CFLAGS) $(TRACE_LIB)

scal: multi_a2a.C
	$(PREP) $(CC) -o multi_a2a.scal $< $(CFLAGS) $(SCALIB)

clean:
	rm -f *.o multi_a2a charmrun
