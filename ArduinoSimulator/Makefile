CP = ../hpms.app.arduinosim/hpms.app.arduinosim.jar:../hpms.app.arduinosim/lib/hpms.fx.angle.jar 

all: BUILD/itepm ../hpms.app.arduinosim/hpms.app.arduinosim.jar bug

ncurses: BUILD/arduinosim

clean:
	rm -fr BUILD
	cd ../hpms.app.arduinosim ; ant clean

BUILD/itepm: BUILD/IfThenElsePourMax.o BUILD/libsim.a
	g++ -L BUILD $< -o $@ -lsim -lpthread -lncurses -lm -lefence
	
BUILD/IfThenElsePourMax.o: ../IfThenElsePourMax/src/IfThenElsePourMax.cpp
	@mkdir -p $$(dirname $@)
	g++ -c -I inc -I ../IfThenElsePourMax/inc $< -o $@

BUILD/libsim.a: BUILD/main.o BUILD/java/UIProxy.o BUILD/ncurses/UI.o
	ar -rcs $@ $?

BUILD/%.o: src/%.cpp
	@mkdir -p $$(dirname $@)
	g++ -W -Wall -g3 -std=c++11 -c -I inc -o $@ $<

../hpms.app.arduinosim/hpms.app.arduinosim.jar:
	cd ../hpms.app.arduinosim ; ant

run-java: ../hpms.app.arduinosim/hpms.app.arduinosim.jar
	make clean all
	java -cp $(CP) hpms.app.arduinosim.Main &
	sleep 2
	BUILD/itepm --ui=java

run-ncurses:
	make clean all
	BUILD/itepm --ui=ncurses 2> ncurses.err
	cat ncurses.err

run-bug:
	make clean all
	BUILD/itepm --ui=ncurses 2> ncurses.err
	cat ncurses.err

bug: src/ncurses/bug.cpp src/ncurses/Window.h
	g++ -W -Wall -g3 -std=c++11 -I inc -o $@ $< -lncursesw

BUILD/main.o        : src/IUI.h src/main.cpp
BUILD/java/UIProxy.o: src/IUI.h src/java/UIProxy.h src/java/UIProxy.cpp
BUILD/ncurses/UI.o  : src/IUI.h src/ncurses/IChangeListener.h src/ncurses/Checkbox.h src/ncurses/Control.h src/ncurses/ControlsPane.h src/ncurses/LogPane.h src/ncurses/Slider.h src/ncurses/Timeout.h src/ncurses/UI.h src/ncurses/Window.h src/ncurses/UI.cpp
