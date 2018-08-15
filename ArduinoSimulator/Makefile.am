SUBDIRS = src

$(top_srcdir)/../hpms.app.arduinosim/hpms.app.arduinosim.jar:
	cd $(top_srcdir)/../hpms.app.arduinosim ; ant

run-java: src/itepm $(top_srcdir)/../hpms.app.arduinosim/hpms.app.arduinosim.jar
	java -cp $(top_srcdir)/../hpms.app.arduinosim/hpms.app.arduinosim.jar:$(top_srcdir)/../hpms.app.arduinosim/lib/hpms.fx.angle.jar hpms.app.arduinosim.Main &
	sleep 2
	src/itepm --ui=java

run-ncurses: src/itepm
	src/itepm --ui=ncurses 2> ncurses.err
	cat ncurses.err

run-ws: src/itepm
	firefox $(top_srcdir)/ws-ui/index.html &
	src/itepm --ui=ws
