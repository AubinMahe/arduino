#include "Serveur.h"

static hpms::Serveur theServeur;

void setup() {
   theServeur.setup();
}

void loop() {
   theServeur.loop();
}
