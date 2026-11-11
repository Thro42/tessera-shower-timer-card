# Duschwasser-Timer Card

Tessera-Kachel für deine vorhandene Duschwasser-Automation in Home Assistant. Die gesamte Kachel schaltet den
gewählten `input_boolean` um. Oben stehen Duschsymbol und Titel, mittig die Restzeit, unten links AN/AUS und unten
rechts die Boilertemperatur. Bei eingeschaltetem Helfer ist die Restzeit rot. Hell- und Dunkeldesign werden unterstützt.

## Einrichtung

1. Deine vorhandene HA-Automation beibehalten: Der `input_boolean` steuert das Aufheizen und den Timer. Das Absenken
   der Boilertemperatur nach Ablauf muss weiterhin Home Assistant übernehmen.
2. Nach dem Hochladen des Projekts die Repository-URL in Tessera unter **Plugins → Mit Link hinzufügen** eintragen:
   `https://github.com/Thro42/tessera-shower-timer-card`. Alternativ die Plugin-Dateien lokal nach
   `/config/tessera-plugins/shower_timer/` kopieren.
3. Plugin dem Display hinzufügen und die Firmware bauen. Benötigt Plugin-API **0.15** oder neuer.
4. Im Layout die Kachel **Duschwasser** hinzufügen und als Hauptentität `input_boolean.duschwassert` wählen.
5. In den Kacheloptionen **Duschwasser-Timer** auf `timer.duchwasser_warm` und **Boilertemperatur** auf
   `sensor.boiler_dhw_current_intern_temperature` setzen. Alle drei Entitäten sind frei wählbar, ohne neuen Build.
6. In der ESPHome-Integration HA-Aktionen für das Display erlauben und die normale Tippaktion beibehalten.
   Layout speichern. **2×1** entspricht den Proportionen deiner Vorlage am besten.

## Verhalten

- Aktiv: sekundengenaue Restzeit aus `finishes_at`; 60 Minuten erscheinen als **60:00**.
- Pausiert: eingefrorene Restzeit aus `remaining`. Inaktiv/abgebrochen: **00:00**.
- Timer fehlt, ist ungültig oder die Display-Uhr ist noch nicht gesetzt: **--:--**.
- Unbekannter Schalterzustand: **--**, kein Schaltbefehl. Temperatur nicht verfügbar: **--°**.
- Temperatur mit einer Nachkommastelle, ohne Umrechnung; °C/°F kompakt als °, Kelvin als K.
- Rot richtet sich ausschließlich nach dem `input_boolean`, auch bei pausiertem Timer.
- Bei sehr schmalen Kacheln werden Symbol/Titel verkürzt. Unterstützte Größen: **1×1 bis 3×2**.
- Deutsch, Englisch, Portugiesisch und Niederländisch. Die Displaysprache wird von Tessera beim Build übernommen.

**Kein Sicherheitsregler:** Die Kachel ersetzt weder die HA-Automation noch deren Abschaltlogik. Bei Verbindungsabbruch
können zuletzt empfangene Zustände sichtbar bleiben. Sie steuert nie direkt den Boiler oder den Timer.

## KI-Beteiligung und Datenschutz

Eine **KI war an Implementierung, Übersetzungen, Dokumentation und Tests beteiligt**. Das Manifest kennzeichnet dies
mit `ai-developed`. Das Plugin arbeitet lokal über Home Assistant ohne Cloudabrufe. Die englische `store.png` entsteht
aus echten Firmware-Vorschau-Renderings mit ausdrücklich synthetischen Testdaten, nicht aus einem KI-Bild.

Technische Details und Teststand: [README.md](README.md) und [TESTING.md](TESTING.md).
