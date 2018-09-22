#!/bin/bash

est_en_marche=${1:-false}
commande="Charger une configuration"
DATA='{"commande":"'${commande}'","argument":{'\
'"est_en_marche":'${est_en_marche}','\
'"vannes":[{'\
'"pin":1,'\
'"ouverte":false,'\
'"matin":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'},'\
'"soir":{'\
'"ouverture":{"heure":22,"minute":0},'\
'"duree":15'\
'}'\
'},{'\
'"pin":2,'\
'"ouverte":false,'\
'"matin":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'},'\
'"soir":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'}'\
'},{'\
'"pin":3,'\
'"ouverte":false,'\
'"matin":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'},'\
'"soir":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'}'\
'},{'\
'"pin":4,'\
'"ouverte":false,'\
'"matin":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'},'\
'"soir":{'\
'"ouverture":{"heure":8,"minute":0},'\
'"duree":15'\
'}'\
'}]}}'
wget \
 --header=Content-type:application/json\
 --post-data="$DATA" \
 --output-document=reponses/charger_une_configuration.json \
 "http://192.168.1.24"
cat reponses/charger_une_configuration.json
echo .
