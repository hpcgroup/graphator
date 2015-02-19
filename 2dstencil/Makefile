CC	= CC

all: jacobi2d 

jacobi2d: jacobi2d.C
	$(CC) -O3 -o jacobi2d $<

trace: jacobi2d.C
	$(CC) -o jacobi2d.trace $< $(CFLAGS) $(TRACE_LIB)

scal: jacobi2d.C
	$(PREP) $(CC) -o jacobi2d.scal $< $(CFLAGS) $(SCALIB)

clean:
	rm -f *.o jacobi2d charmrun
