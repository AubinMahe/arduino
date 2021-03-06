cmake_minimum_required (VERSION 2.6)
project ( AutomatePourMax )
add_definitions(
  -DF_CPU=16000000L
  -DPLATFORMIO=30600
  -DARDUINO_AVR_UNO
  -DARDUINO_ARCH_AVR
  -DARDUINO=10805
  -D__AVR_ATmega328P__
)
include_directories(
  ../lib/utils
  ../lib/fsm
  /home/aubin/.platformio/packages/framework-arduinoavr/cores/arduino
  /home/aubin/.platformio/packages/framework-arduinoavr/variants/standard
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/EEPROM/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/HID/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/SPI/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/SoftwareSerial/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/Wire/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Adafruit_CircuitPlayground
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Adafruit_CircuitPlayground/utility
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Bridge/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Esplora/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Ethernet/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Firmata
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Firmata/utility
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/GSM/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Keyboard/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/LiquidCrystal/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Mouse/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/RobotIRremote/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Robot_Control/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Robot_Motor/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/SD/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Servo/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/SpacebrewYun/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Stepper/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/TFT/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/Temboo/src
  /home/aubin/.platformio/packages/framework-arduinoavr/libraries/WiFi/src
  /home/aubin/.platformio/packages/toolchain-atmelavr/avr/include
  /home/aubin/.platformio/packages/toolchain-atmelavr/lib/gcc/avr/4.9.2/include-fixed
  /home/aubin/.platformio/packages/toolchain-atmelavr/lib/gcc/avr/4.9.2/include
  /home/aubin/.platformio/packages/tool-unity
)
add_library   ( utils
  lib/utils/ActionsDifferees.cpp
  lib/utils/Bouton.cpp
)
add_library   ( fsm
  lib/fsm/Automate_impl.cpp
  lib/fsm/EvenementsTemporels_impl.cpp
)
add_executable( AutomatePourMax
  src/AutomatePourMax.cpp
)
