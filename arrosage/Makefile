CXX      = g++
LINK.o   = $(LINK.cc)
CXXFLAGS = -std=c++11 -pedantic -W -Wall -g3 -O0\
 -I lib/json\
 -I ../ArduinoSimulator/src\
 -I ../ArduinoSimulator/src/arduinosim
LDFLAGS  = -L ../ArduinoSimulator/BUILD -lsim -lwebsockets -lpthread -lncursesw -lefence
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

www/index-min.html.gz: www/index.html
	html-minifier $(MINIFIER_FLAGS) $< | gzip >$@

www/arrosage-min.css.gz: www/arrosage.css
	php minify.php

www/arrosage-min.js.gz: www/arrosage.js
	php minify.php

src/%.gz.h: www/%.gz
	xxd -i $<                                                   $@
	sed 's/unsigned char/static const unsigned char/g' $@     > $@.tmp
	sed 's/unsigned int/static const unsigned int/g'   $@.tmp > $@
	rm -f $@.tmp

src/index-min.html.gz.h: www/index-min.html.gz
src/arrosage-min.css.gz.h: www/arrosage-min.css.gz
src/arrosage-min.js.gz.h: www/arrosage-min.js.gz
BUILD/src/Serveur.o: src/index-min.html.gz.h src/arrosage-min.css.gz.h src/arrosage-min.js.gz.h src/Serveur.h src/Serveur.cpp
