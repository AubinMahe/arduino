#!/bin/bash

function check {
   "$@"
   local status=$?
   if [ $status -ne 0 ]; then
      echo "error with $1" >&2
      exit $status
   fi
}

rm -fr build
mkdir -p build/build/build
check cd build/build/build
chmod -fR 777 src
(cd ../../.. ; check autoreconf --install --force)
check ../../../configure
check make distcheck
rm -fr ../../download
check mkdir -p ../../download
check mv arduinosim-*.tar.gz ../../download
check cd ../../download
check tar xzf arduinosim-*.tar.gz
check cd $(find -type d -name 'arduinosim-*')
check ./configure --prefix $(readlink -f .)/install-dir
check make
check make install
check find install-dir

