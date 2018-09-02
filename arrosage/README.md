# Arrosage (Internet Of Things) #

Commande des quatre électro-vannes d'arrosage de ma pelouse.

La carte utilisée est une WeMos® D1 Mini V2 NodeMcu 4M Bytes WIFI ESP8266.

Le WiFi permet d'héberger un serveur HTTP pour recevoir les commandes suivantes :

* Requête d'interrogation pour relever la configuration active.
* Commande arrêt / marche du système dans son ensemble.
* Commande ponctuelle : arrêt / marche d'une ou plusieurs vannes.
* Configuration du système dans son ensemble : heures des arrêt / marche,
  vanne par vanne, pour toutes les vannes.

Les commandes ainsi que les retour d'état sont encodés en JSON.

La partie cliente reste à faire, j'envisage plusieurs clients :

* JavaFx : l'application commence par relever la configuration active via HTTP,
l'affiche puis offre la possibilité de commander ponctuellement une vanne ou
de télécharger une nouvelle configuration  

* Navigateur HTML5 : même finalité mais avec des moyens différents.
