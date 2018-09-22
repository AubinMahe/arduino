#!/bin/bash

etat=${1:-false}
wget \
 --header=Content-type:application/json\
 --post-data='{"commande":"Démarrer ou arrêter","argument":{"demarrer":'$etat'}}' \
 --output-document=reponses/démarrer_ou_arrêter.json \
 "http://192.168.1.24"
cat reponses/démarrer_ou_arrêter.json
echo .
