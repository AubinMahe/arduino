CXX      = g++
LINK.o   = $(LINK.cc)
CXXFLAGS = -std=c++11 -pedantic -W -Wall -g3 -O0 -DTESTS_UNITAIRES
LDFLAGS  = -lefence
SRCS     = Attributes.cpp Decoder.cpp Encoder.cpp IJSonData.cpp JSonTests.cpp
OBJS     = $(SRCS:%.cpp=%.o)

all: JSonTests

clean:
	rm -f JSonTests *.o

valgrind: JSonTests
	valgrind --leak-check=full --show-leak-kinds=all ./JSonTests ALL

tests: JSonTests
	./JSonTests ALL

JSonTests: $(OBJS)

Attributes.o: Attributes.cpp Attributes.h IJSonData.h Status.h Encoder.h \
 Decoder.h
Decoder.o: Decoder.cpp Decoder.h Attributes.h IJSonData.h Status.h
Encoder.o: Encoder.cpp Encoder.h Attributes.h IJSonData.h Status.h
IJSonData.o: IJSonData.cpp IJSonData.h Status.h Encoder.h Attributes.h
JSonTests.o: JSonTests.cpp Encoder.h Attributes.h IJSonData.h Status.h
