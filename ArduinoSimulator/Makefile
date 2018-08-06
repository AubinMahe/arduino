CCC = g++
CP  = ../hpms.app.arduinosim/hpms.app.arduinosim.jar:../hpms.app.arduinosim/lib/hpms.fx.angle.jar 

all: BUILD/itepm ../hpms.app.arduinosim/hpms.app.arduinosim.jar

clean:
	rm -fr BUILD
	cd ../hpms.app.arduinosim ; ant clean

BUILD/itepm: BUILD/IfThenElsePourMax.o BUILD/libsim.a
	g++ -L BUILD $< -o $@ -lsim -lpthread

BUILD/IfThenElsePourMax.o: ../IfThenElsePourMax/src/IfThenElsePourMax.cpp
	@mkdir -p BUILD
	g++ -c -I inc -I ../IfThenElsePourMax/inc $< -o $@

BUILD/libsim.a: src/sim.cpp
	@mkdir -p BUILD
	g++ -c -I inc -o BUILD/sim.o src/sim.cpp
	ar -rcs BUILD/libsim.a BUILD/sim.o

../hpms.app.arduinosim/hpms.app.arduinosim.jar:
	cd ../hpms.app.arduinosim ; ant

run: ../hpms.app.arduinosim/hpms.app.arduinosim.jar
	make clean all
	#gnome-terminal -e 'java -jar ../hpms.app.arduinosim/hpms.app.arduinosim.jar' &
	java -cp $(CP) hpms.app.arduinosim.Main &
	sleep 2
	BUILD/itepm
