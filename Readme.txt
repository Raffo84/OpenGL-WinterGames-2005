   -=- OpenGL WinterGames 2005 -=-

1. DIE MENÜS

Über "Neues Spiel" kann man ein lokales Spiel mit Bots starten, wahlweise "Freestyle", "Downhill" oder "Slalom".
Außerdem kann man sich zwischen einer normalen und einer Nachtfahrt entscheiden.

Für die "Netzwerkspiele" kommt man in eine Lobby, in der alle Spieler und Bots angezeigt werden, die im Spiel
sind. Als Client kann man hier momentan nur abwarten und sich als "Bereit" melden oder auf "Abbrechen" gehen,
als Server kann man den Spieltyp zwischen "Downhill" oder "Freestyle" wechseln, man kann Bots hinzufügen und
Spieler (oder Bots) wieder aus dem Spiel kicken oder die Uhrzeit wechseln. Als Server bekommt man auch seine
lokale(n) IP-Adresse(n) angezeigt, die man dann recht einfach an seine Spieler weitergeben kann.

Unter den "Optionen" kann man seinen "Spieler" inklusive Namen einstellen, man kann ein paar "Video"-Einstellungen
ändern, man kann sich die "Credits" anschauen und die Bestzeiten anschauen oder löschen.

2. DIE STEUERUNG

gesteuert wird im Spiel über die Cursortasten. Mit den Tasten Pfeil-Nach-Links und -Rechts dreht man seinen Spieler
entsprechend, mit Pfeil-Nach-Unten bremst man ab. Mit der Pfeil-Nach-Oben-Taste aktiviert man den Turbo Boost.
Dieser lädt sich erst voll auf und baut danach wieder ab.
Wenn man über eine Kante fährt, kann es leicht sein, dass man abhebt. Wenn der Turbo Boost dann aktiviert ist, bekommt
man einen gewissen Sprungbonus, je nachdem, wie voll der Booster geladen ist.

3. DIE SPIELMODI

3.1 FREESTYLE
Im Freestyle-Modus kommt es darauf an, möglichst viele Style-Punkte (SP) zu sammeln. Man kann auf drei Arten SP
bekommen, sie alle haben aber mit Sprüngen zu tun.

Die einfachste Art ist, nur zu springen. Während man in der Luft ist, bekommt man SP, je länger, desto mehr.
Wenn man hoch genug fliegt, kann man versuchen, sich in der Luft zu drehen, dann bekommt man für je 90° einen
Multiplikator, also für eine 90°-Drehung x2, für 180° x3, für 360° x4 u.s.w. Dieser Multiplikator wird bei der
Landung auf die neuen Punkte angerechnet. Wenn man also bei einem 500-Punkte-Sprung einen 360 schafft, bekommt
man tatsächlich 2000 Punkte! Aber vorsicht: Wenn man seitlich oder rückwärts zur Flugrichting landet (also zwischen
90° und 270°), baut man einen Unfall und die Punkte für den Sprung werden abgezogen! Wenn der 360 beim Sprung
von oben also nicht klappt und man nur 180° schafft, bekommt man die 500 Punkte abgezogen.

Eine andere Möglichkeit an Punkte zu kommen besteht in sehr Hohen Sprüngen. Wenn man es schafft, eine Sprunghöhe
von über 50m zu springen, bekommt man für jeden weiteren Höhenmeter Punkte, ab 100m bekommt man sogar doppelte
Punkte. Es gilt aber immer nur der höchste Sprung, das heißt, nach einem Sprung über 75m bekommt man keine
Höhenpunkte mehr für einen Sprung über 60m.

Eine Dritte Möglichkeit besteht im Turbo Boost: Wenn man einen Absprung schafft, wenn der Booster gerade voll
aufgeladen ist, bekommt man Bonuspunkte.

Diese drei Möglichkeiten lassen sich natürlich frei kombinieren: Einen Absprung mit voll geladenem Booster über
eine Schlucht um auf 100 Höhenmeter oder mehr zu kommen, während man sich um 360° oder mehr dreht bringt enorm
Punkte.

Ein Freestyle Spiel endet, wenn man unter die Schneegrenze fährt oder nach spätestens 1:30 min.

3.2 DOWNHILL
Bei einem Downhill-Rennen kommt es nur darauf an, möglichst schnell an die Schneegrenze zu fahren.

Da man auf steileren Strecken schneller beschleunigt, als auf flachen Strecken, ist die Wahl der Fahrtroute für
die Zeit entscheidend. Auch ein wohl platzierter Sprung kann sich auf die Zeit auswirken: entweder positiv
durch einen Geschwindigkeitsbonus oder auch negativ, wenn man falsch landet oder ungünstig über steile Stellen
oder das Ziel drüber springt.

3.3 SLALOM (nur Singleplayer)
Im Slalom werden 5 Tore auf dem Berg verteilt, durch die man möglichst schnell fahren muss. Achtung: man muss
*durch* die Tore fahren, nicht wie beim anderen Slalom an den Pfosten vorbei (wobei man da auch eigentlich durch
Tore fährt nur sind die Tore so breit, dass niemand den anderen Pfosten bemerkt...).

Über dem Kopf des Spielers erscheint ein Pfeil, der die Richtung zum nächsten Gate anzeigt, außerdem wird das
aktuelle Gate durch einen Pfeil über dem Tor angezeigt. Es kann immer nur das aktuelle Gate gewertet werden,
es bringt also nichts, die Tore in falscher Reihenfolge zu durchfahren. Jedes Tor hat außerdem einen Time-Out,
wenn man also an einem Gate hoffnungslos vorbeifährt, kann man meistens einfach weiter fahren und darauf warten,
dass das vorherige Tor entwertet wird. Aber Achtung: jedes verpasste Gate bringt 10 Sekunden Strafzeit am Schluss!

4. DIE ANZEIGEN

Oben links ist ein Geschwindigkeitsanzeiger, der die aktuelle Geschwindigkeit in km/h angibt. Am rechten
Bildschirmrand ist ein Höhenanzeiger, der die aktuelle (und bei Freestyle die höchste) Sprunghöhe anzeigt.
Der Ladestand des Turbo Boosters wird unten links angezeigt.

4.1 ANZEIGEN IM FREESTYLE-MODUS
Oben rechts ist eine kleine Liste, die alle Spieler und ihre aktuellen Punkte anzeigt. Oben links werden die
gesammelten Style-Punkte angezeigt und bei (und kurz nach) einem Sprung die Punkte, die man in diesem Sprung
schon bekommt, inkl. Multiplikator (oder, nach einem Sturz, welchen Abzug man bekommt).

4.2 ANZEIGEN IM DOWNHILL-MODUS
Oben links unter dem Tachometer wird angezeigt, wie lange man schon unterwegs ist.

4.3 ANZEIGEN IM SLALOM
Unter dem Tachometer wird zum einen Angezeigt, wie lange das aktuelle Gate noch aktiv ist, bevor der Time-Out
einsetzt, zum anderen wird angezeigt, welches Gate von wie viele gerade aktiv ist.

5. NETZWERKMODUS

Die WinterGames verwenden eine TCP-Verbindung auf Port 2143 für Netzwerkspiele. Dieser Port muss von Firewalls
freigeschalten (und evtl. von Routern durchgelassen) werden, damit man im Netzwerk oder über das Internet
spielen kann.

6. DIE AUTOREN

Das Ganze wurde programmiert von
	FLORIAN KRÜGER (kruegerf@in.tum.de) und
	RALF STAUDER (stauder@in.tum.de).

