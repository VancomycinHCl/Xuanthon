objects = test.o dynarray.o
exe = test

#test: test.o dynarray.o
	#cc -o test test.o dynarray.o
test: $(objects)
	cc -o test $(objects)
test.o: test.c
	cc -c test.c
dynarray.o: dynarray.c dynarray.h
	cc -c dynarray.c
.PHONY: clean
clean:
	rm $(objects)
	rm $(exe)
