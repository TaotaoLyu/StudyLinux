.PHONY:all
all:mylib 


mylib:libmymath.a my_add.h my_sub.h
	mkdir -p mylib/include
	mkdir -p mylib/lib 
	mv libmymath.a mylib/lib 
	cp *.h mylib/include
	rm *.o

libmymath.a:my_add.o my_sub.o
	ar -rc $@ $^
my_add.o:my_add.c
	gcc -c $^
my_sub.o:my_sub.c
	gcc -c $^


.PHONY:clean
clean:
	rm -rf mylib
