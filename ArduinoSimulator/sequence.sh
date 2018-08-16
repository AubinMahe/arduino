#!/bin/bash

function check {
   "$@"
   local status=$?
   if [ $status -ne 0 ]; then
      echo "error with $1" >&2
      exit $status
   fi
}

mkdir -p /home/aubin/Dev/Arduino/arduino/ArduinoSimulator/build/build/build
check cd /home/aubin/Dev/Arduino/arduino/ArduinoSimulator/build/build/build
chmod -fR 777 arduinosim-1.0 src
rm -fr arduinosim-1.0
rm -f config.h
rm -f config.log
rm -f config.status
rm -f Makefile
rm -fr src
rm -f stamp-h1
(cd ../../.. ; check autoreconf --install --force)
check ../../../configure
check make distcheck
check mv arduinosim-1.0.tar.gz ../../download
check cd ../../download
check chmod -R 777 arduinosim-1.0
check rm -fr arduinosim-1.0
check tar xzf arduinosim-1.0.tar.gz
check cd arduinosim-1.0
check ./configure --prefix /home/aubin/Dev/Arduino/arduino/ArduinoSimulator/build/download/arduinosim-1.0/install-dir
check make
check make install
check find install-dir
