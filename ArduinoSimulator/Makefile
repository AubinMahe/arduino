# Makefile for Eclipse
#----------------------
CXX = g++

CXXFLAGS  = -W -Wall -g3 -O0 -std=c++11 -I src/arduinosim -DDEBUG
#CXXFLAGS = -W -Wall -g3 -O3 -std=c++11 -I src/arduinosim -DDEBUG
#CXXFLAGS = -W -Wall -g0 -O3 -std=c++11 -I src/arduinosim -DNDEBUG

LDFLAGS = -L BUILD -lsim -lncursesw -lm -lwebsockets -lpthread -lefence

SRCS =\
 src/main.cpp\
 src/IUI.cpp\
 src/arduinosim/ESP8266WiFiClass.cpp\
 src/arduinosim/EspClass.cpp\
 src/arduinosim/FS.cpp\
 src/arduinosim/IPAddress.cpp\
 src/arduinosim/NTPClient.cpp\
 src/arduinosim/Print.cpp\
 src/arduinosim/String.cpp\
 src/arduinosim/WiFiClient.cpp\
 src/arduinosim/WiFiServer.cpp\
 src/java/UIProxy.cpp\
 src/ncurses/UI.cpp\
 src/ncurses/getFileContents.cpp\
 src/ncurses/utf8len.cpp\
 src/ws/WebSocketUIServer.cpp\
 src/ws/jsontest.cpp

OBJS = $(SRCS:%.cpp=BUILD/%.o)

all: BUILD/arduinosimsample $(OBJS)

clean:
	rm -fr BUILD
	rm -f  arduinosimsample.err
	rm -f  Makefile.deps
	rm -f  src/arduinosim/IPAddressValue.h
	make Makefile.deps

src/arduinosim/IPAddressValue.h:
	ifconfig | grep -w inet | grep broadcast | awk ' { split( $$2, parts, "." ); print "static uint8_t LOCAL_IP_ADDRESS[] = { " parts[1] ", " parts[2] ", " parts[3] ", " parts[4] " };" }' > $@

BUILD/arduinosimsample: BUILD/IfThenElsePourMax.o BUILD/libsim.a
	$(CXX) $< -o $@ $(LDFLAGS)
	
BUILD/IfThenElsePourMax.o: src/sample/IfThenElsePourMax.cpp
	@mkdir -p $$(dirname $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

BUILD/libsim.a: $(OBJS)
	ar -rcs $@ $?

BUILD/%.o: %.cpp
	@mkdir -p $$(dirname $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run-java: BUILD/arduinosimsample
	java -jar java-ui/hpms.app.arduinosim.jar 2> arduinosimsample.err &
	sleep 2
	BUILD/arduinosimsample --ui=java
	cat arduinosimsample.err

run-ncurses: BUILD/arduinosimsample
	BUILD/arduinosimsample --ui=ncurses 2> arduinosimsample.err
	cat arduinosimsample.err

run-ws: BUILD/arduinosimsample
	firefox ws-ui/index.html &
	BUILD/arduinosimsample --ui=ws

Makefile.deps: $(SRCS) src/arduinosim/IPAddressValue.h
	$(CXX) $(CXXFLAGS) -MM $(SRCS) |	gawk -v PREFIX=BUILD -f Makefile.gawk > Makefile.deps

-include Makefile.deps
