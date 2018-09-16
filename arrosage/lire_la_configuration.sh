#!/bin/bash

wget \
 --header=Content-type:application/json\
 --post-data='{"commande": "Lire la configuration"}' \
 --output-document=reponses/lire_la_configuration.json \
 "http://192.168.1.24"
cat reponses/lire_la_configuration.json
echo .
