# arduino
Partage d'un projet Arduino avec [Max](https://github.com/exancillatus) (Ctrl-Clic pour ouvrir un nouvel onglet)

Pour consulter la documentation de IfThenElsePourMax, c'est
[par ici !](https://aubinmahe.github.io/arduino/IfThenElsePourMax/doc/html/index.html) (Ctrl-Clic pour ouvrir un nouvel onglet)

J'ai développé [Un simulateur minimaliste](ArduinoSimulator) pour Arduino UNO avec [une IHM](hpms.app.arduinosim) en Java et [une autre](ArduinoSimulator/src/ncurses) au moyen de *ncurses*.

* Il route les appels aux routines Arduino vers des appels systèmes UNIX, les entrées/sorties sont simulées au moyen des interfaces, JavaFX ou ncurses.

* Attention, pour simuler un appui bouton, il faut cocher puis décocher, pour bien voir les deux états enfoncé/relâché d'un bouton poussoir.
