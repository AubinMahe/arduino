CXX = g++

LINK.o = $(LINK.cc)

CXXFLAGS = -std=c++11 -pedantic -W -Wall -g3 -O0\
 -I lib/json\
 -I ../ArduinoSimulator/src\
 -I ../ArduinoSimulator/src/arduinosim

LDFLAGS = -L ../ArduinoSimulator/BUILD -lsim -lwebsockets -lpthread -lncursesw -lefence

MINIFIER_FLAGS =\
 --collapse-whitespace\
 --remove-comments\
 --remove-optional-tags\
 --remove-redundant-attributes\
 --remove-script-type-attributes\
 --use-short-doctype\
 --minify-css true\
 --minify-js true

SRCS =\
 lib/json/IJSonData.cpp\
 lib/json/Attributes.cpp\
 lib/json/Decoder.cpp\
 lib/json/Encoder.cpp\
 src/Activite.cpp\
 src/Arrosage.cpp\
 src/Commandes.cpp\
 src/Horloge.cpp\
 src/Instant.cpp\
 src/Journal.cpp\
 src/Log.cpp\
 src/Serveur.cpp\
 src/Vanne.cpp

OBJS = $(SRCS:%.cpp=BUILD/%.o)

all: BUILD/arrosage

BUILD:
	mkdir -p BUILD

clean:
	rm -fr BUILD
	rm -fr build
	rm -fr ../ArduinoSimulator/BUILD
	rm -fr ../ArduinoSimulator/build
	rm -f  www/*.gz
	rm -f  Makefile.deps
	make Makefile.deps

valgrind: arrosage
	valgrind --leak-check=full --show-leak-kinds=all BUILD/arrosage --ui=ws --serial-to-stderr

run: BUILD/arrosage
	BUILD/arrosage --ui=ws --serial-to-stderr
# --dump-http=true

../ArduinoSimulator/BUILD/libsim.a:
	cd ../ArduinoSimulator ; make

BUILD/arrosage: $(OBJS) ../ArduinoSimulator/BUILD/libsim.a
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

BUILD/%.o: %.cpp
	@mkdir -p $$(dirname $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

data/index-min.html.gz: www/index.html
	html-minifier $(MINIFIER_FLAGS) $< | gzip >$@

data/arrosage-min.css.gz data/arrosage-min.js.gz: www/arrosage.css www/arrosage.js
	php minify.php

Makefile.deps: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $(SRCS) |	gawk -v PREFIX=BUILD -f Makefile.gawk > Makefile.deps

-include Makefile.deps
