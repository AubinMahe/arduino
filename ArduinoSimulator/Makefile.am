SUBDIRS = src

EXTRA_DIST = \
 java-ui/hpms.app.arduinosim.jar \
 java-ui/hpms.fx.angle.jar \
 ws-ui/css/arduinosim.css \
 ws-ui/js/arduinosim.js \
 ws-ui/favicon.ico \
 ws-ui/index.html

nobase_dist_pkgdata_DATA = \
 java-ui/hpms.app.arduinosim.jar \
 java-ui/hpms.fx.angle.jar \
 ws-ui/css/arduinosim.css \
 ws-ui/js/arduinosim.js \
 ws-ui/favicon.ico \
 ws-ui/index.html \
 instructions.txt

run-java: src/itepm java-ui/hpms.app.arduinosim.jar
	java -cp java-ui/hpms.app.arduinosim.jar:java-ui/hpms.fx.angle.jar hpms.app.arduinosim.Main &
	sleep 2
	src/itepm --ui=java

run-ncurses: src/itepm
	src/itepm --ui=ncurses 2> ncurses.err
	cat ncurses.err

run-ws: src/itepm
	firefox ws-ui/index.html &
	src/itepm --ui=ws
