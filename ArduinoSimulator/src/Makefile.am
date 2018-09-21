AM_CXXFLAGS = -I$(top_srcdir)/src/arduinosim

pkginclude_HEADERS = \
 arduinosim/Arduino.h \
 arduinosim/Servo.h 

lib_LIBRARIES = libarduinosim.a
libarduinosim_a_SOURCES = \
 main.cpp \
 IUI.h \
 IUI.cpp \
 java/UIProxy.h \
 java/UIProxy.cpp \
 ncurses/UI.cpp \
 ncurses/getFileContents.cpp \
 ncurses/LogPane.h \
 ncurses/Slider.h \
 ncurses/UI_Impl.h \
 ncurses/Checkbox.h \
 ncurses/UI.h \
 ncurses/utf8len.h \
 ncurses/StatusPane.h \
 ncurses/IChangeListener.h \
 ncurses/getFileContents.h \
 ncurses/Control.h \
 ncurses/ControlsPane.h \
 ncurses/Window.h \
 ncurses/Timeout.h \
 ws/json.h \
 ws/WebSocketServer_Impl.h \
 ws/WebSocketUIServer.h \
 ws/WebSocketUIServer.cpp

bin_PROGRAMS             = arduinosimsample
arduinosimsample_SOURCES = sample/IfThenElsePourMax.cpp
arduinosimsample_LDADD   = libarduinosim.a
