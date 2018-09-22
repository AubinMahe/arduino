#!/bin/bash

pin1=${1:-1}
eta1=${2:-false}
pin2=${3:-2}
eta2=${4:-false}
pin3=${5:-3}
eta3=${6:-false}
pin4=${7:-4}
eta4=${8:-false}
COMMAND="Ouvrir ou fermer les vannes"
DATA='{"commande":"'${COMMAND}'","argument":{"les_vannes":['\
'{"pin":'$pin1',"ouvrir":'$eta1'},'\
'{"pin":'$pin2',"ouvrir":'$eta2'},'\
'{"pin":'$pin3',"ouvrir":'$eta3'},'\
'{"pin":'$pin4',"ouvrir":'$eta4'}]}}'
wget \
 --header=Content-type:application/json\
 --post-data="$DATA" \
 --output-document=reponses/ouvrir_ou_fermer_les_vannes.json \
 "http://192.168.1.24"
cat reponses/ouvrir_ou_fermer_les_vannes.json
echo .
