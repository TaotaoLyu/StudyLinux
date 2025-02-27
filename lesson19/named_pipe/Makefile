.PHONY:all
all:client.out server.out


client.out:client.cc
	g++ -o $@ $^

server.out:server.cc
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f server.out client.out