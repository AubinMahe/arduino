CXX      = g++
LINK.o   = $(LINK.cc)
CXXFLAGS = -std=c++11 -pedantic -W -Wall -g3 -O0 -I lib -I ../ArduinoSimulator/src/arduinosim
LDFLAGS  = -L ../ArduinoSimulator/BUILD -lsim -lwebsockets -lpthread -lncursesw -lefence
SRCS     =\
 lib/json/IJSonData.cpp\
 lib/json/Attributes.cpp\
 lib/json/Decoder.cpp\
 lib/json/Encoder.cpp\
 src/Vanne.cpp\
 src/Serveur.cpp\
 src/Horloge.cpp\
 src/Arrosage.cpp\
 src/Instant.cpp\
 src/Commandes.cpp\
 src/Activite.cpp
 
OBJS = $(SRCS:%.cpp=BUILD/%.o)

all: BUILD/arrosage

BUILD:
	mkdir -p BUILD

clean:
	rm -fr BUILD
	rm -fr build
	rm -fr ../ArduinoSimulator/BUILD
	rm -fr ../ArduinoSimulator/build

valgrind: arrosage
	valgrind --leak-check=full --show-leak-kinds=all BUILD/arrosage

../ArduinoSimulator/BUILD/libsim.a:
	cd ../ArduinoSimulator ; make

BUILD/arrosage: $(OBJS) ../ArduinoSimulator/BUILD/libsim.a
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

BUILD/%.o: %.cpp
	@mkdir -p $$(dirname $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/index.html.h: index.html
	xxd -i $<                                                   $@.tmp
	sed 's/unsigned char/static const unsigned char/g' $@.tmp > $@
	sed 's/unsigned int/static const unsigned int/g'   $@     > $@.tmp
	sed 's/};/,0};/g'                                  $@.tmp > $@
	rm -f $@.tmp

BUILD/src/Serveur.o: src/index.html.h src/Serveur.cpp
