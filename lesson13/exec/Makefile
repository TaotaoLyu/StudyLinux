
.PHONY:all
all:mycmd myexec
	



myexec:myexec.c
	gcc -o $@ $^

mycmd:mycmd.c
	gcc -o $@ $^


.PHONY:clean
clean:
	rm -f myexec
