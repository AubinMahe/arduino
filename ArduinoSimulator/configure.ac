AC_INIT([arduinosim],[1.0],[])
AM_INIT_AUTOMAKE([subdir-objects foreign -Wall -Werror])
AM_PROG_AR
AX_CXX_COMPILE_STDCXX_11(noext)
AC_PROG_CXX
AC_PROG_RANLIB
AC_CHECK_FUNCS([gettimeofday]) 
AC_CHECK_FUNCS([isascii])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([setlocale])
AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([strtol])
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_FUNC_STRTOD
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_SEARCH_LIBS([lws_create_context], [websockets], [], [AC_MSG_ERROR([library 'libwebsockets' is required for ArduinoSim])])
AC_SEARCH_LIBS([initscr], [ncursesw], [], [AC_MSG_ERROR([library 'cursesw' is required for ArduinoSim])])
AC_SEARCH_LIBS([pthread_create], [pthread], [], [AC_MSG_ERROR([library 'pthread' is required for ArduinoSim])])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_SUBST([CXXFLAGS], ['-g0 -O3 -std=c++11'])
AC_OUTPUT
