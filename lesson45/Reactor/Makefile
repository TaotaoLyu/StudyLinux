
LOG=LOGTOFILE


TcpServer:Main.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp -D$(LOG)

.PHONY:clean
clean:
	rm -f TcpServer