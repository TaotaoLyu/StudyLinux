
cc=g++
#LD=-DMYSELF 
LD=-DJSON


.PHONY:ALL
ALL:calclient calserver


calclient:calClient.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp $(LD)

calserver:calServer.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp $(LD)

.PHONY:clean
clean:
	rm -f calclient calserver
