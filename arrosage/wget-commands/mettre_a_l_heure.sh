#!/bin/bash

heure=${1:-14}
minute=${2:-45}
COMMAND="Mettre à l'heure"
DATA='{"commande":"'${COMMAND}'","argument":{"heure":'$heure',"minute":'$minute'}}'
wget \
 --header=Content-type:application/json\
 --post-data="$DATA" \
 --output-document=reponses/mettre_a_l_heure.json \
 "http://192.168.1.24"
cat reponses/mettre_a_l_heure.json
echo .
