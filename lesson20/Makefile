.PHONY:all
all:server.out client.out


server.out:server.cc 
	g++ -o $@ $^ -std=c++11

client.out:client.cc
	g++ -o $@ $^ -std=c++11
	
.PHONY:clean
clean:
	rm -f client.out server.out