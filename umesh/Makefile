CC = CC

all: unstructured 

unstructured: unstructured.C
	$(CC) -O3 -o unstructured $< 

trace: unstructured.C
	$(CC) -o unstructured.trace $< $(CFLAGS) $(TRACE_LIB)

scal: unstructured.C
	$(PREP) $(CC) -o unstructured.scal $< $(CFLAGS) $(SCALIB)

clean:
	rm -f *.o unstructured charmrun
