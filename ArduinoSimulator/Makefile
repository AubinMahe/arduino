# Makefile for Eclipse
#----------------------
CXX      = g++
CXXFLAGS = -W -Wall -g3 -O0 -std=c++11 -I src/arduinosim -DDEBUG
#CXXFLAGS = -W -Wall -g3 -O3 -std=c++11 -I src/arduinosim -DDEBUG
#CXXFLAGS = -W -Wall -g0 -O3 -std=c++11 -I src/arduinosim -DNDEBUG
LDFLAGS  = -L BUILD -lsim -lncursesw -lm -lwebsockets -lpthread -lefence
OBJS     =\
 BUILD/main.o\
 BUILD/IUI.o\
 BUILD/arduinosim/ESP8266WiFiClass.o\
 BUILD/arduinosim/EspClass.o\
 BUILD/arduinosim/IPAddress.o\
 BUILD/arduinosim/NTPClient.o\
 BUILD/arduinosim/WiFiClient.o\
 BUILD/arduinosim/WiFiServer.o\
 BUILD/java/UIProxy.o\
 BUILD/ncurses/UI.o\
 BUILD/ncurses/getFileContents.o\
 BUILD/ncurses/utf8len.o\
 BUILD/ws/WebSocketUIServer.o\
 BUILD/ws/jsontest.o

all: BUILD/arduinosimsample $(OBJS)

clean:
	rm -fr BUILD arduinosimsample.err

BUILD/arduinosimsample: BUILD/IfThenElsePourMax.o BUILD/libsim.a
	$(CXX) $< -o $@ $(LDFLAGS)
	
BUILD/IfThenElsePourMax.o: src/sample/IfThenElsePourMax.cpp
	@mkdir -p $$(dirname $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

BUILD/libsim.a: $(OBJS)
	ar -rcs $@ $?

BUILD/%.o: src/%.cpp
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

BUILD/java/UIProxy.o: src/IUI.h src/java/UIProxy.h src/java/UIProxy.cpp
BUILD/ncurses/UI.o: src/IUI.h src/ncurses/UI_Impl.h src/ncurses/utf8len.h src/ncurses/getFileContents.h src/ncurses/IChangeListener.h src/ncurses/Checkbox.h src/ncurses/Control.h src/ncurses/ControlsPane.h src/ncurses/LogPane.h src/ncurses/Slider.h src/ncurses/Timeout.h src/ncurses/UI.h src/ncurses/Window.h src/ncurses/UI.cpp
BUILD/ncurses/getFileContents.o: src/ncurses/getFileContents.cpp src/ncurses/getFileContents.h
BUILD/ws/WebSocketUIServer.o: src/IUI.h src/ws/json.h src/ws/WebSocketServer_Impl.h src/ws/WebSocketUIServer.h
BUILD/main.o: src/arduinosim/Arduino.h src/arduinosim/Servo.h src/IUI.h src/main.cpp
BUILD/IfThenElsePourMax.o: src/arduinosim/Arduino.h src/arduinosim/Servo.h src/sample/IfThenElsePourMax.cpp
