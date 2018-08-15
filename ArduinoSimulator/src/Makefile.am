AM_CXXFLAGS = -I$(top_srcdir)/inc
lib_LIBRARIES = libsim.a
libsim_a_SOURCES = \
 java/UIProxy.cpp \
 ncurses/UI.cpp \
 ncurses/getFileContents.cpp \
 ws/WebSocketUIServer.cpp
bin_PROGRAMS = itepm
itepm_SOURCES = main.cpp ../../IfThenElsePourMax/src/IfThenElsePourMax.cpp
itepm_LDADD = libsim.a
