
cc=g++
.PHONY:ALL
ALL:tcpclient tcpserver


tcpclient:tcpclient.cc
	$(cc) -o $@ $^ -std=c++11

tcpserver:tcpserver.cc
	$(cc) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpclient tcpserver
