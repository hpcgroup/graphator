CC	= CC

all: jacobi4d 

jacobi4d: jacobi4d.C
	$(CC) -O3 -o jacobi4d $< 

trace: jacobi4d.C
	$(CC) -o jacobi4d.trace $< $(CFLAGS) $(TRACE_LIB)

scal: jacobi4d.C
	$(PREP) $(CC) -o jacobi4d.scal $< $(CFLAGS) $(SCALIB)

clean:
	rm -f *.o jacobi4d charmrun
