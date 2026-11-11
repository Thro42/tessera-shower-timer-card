# Kurtz Beschreibung

Ich möchte das du ein Tessera Plugin generierst das die Steuerung meines Duschwasser Timers ermöglicht. 
Ich nutze für das Aufheizen meines Duschwasser eine Automation ind Homeassistant. Hier für nutze ich einige Helfer. Für das An- und Aus-schalten habe ich einen input_boolean Helfer angelegt. Dieser steuert gleich mehrere dinge. Zum einen wird die Boilertemperatur damit herraufgesetzt und zum anderen startet er einen Timer. Wird der input_boolean Helfer wieder Ausgeschaltet, wird die Boilertemperatur wieder herunter gesetzt und der Timer gestoppt. Der Timer läuft für eine 60:00 Minuten. Wenn er abgelaufem ist, wir ebenfalls die Boilertemperatur wieder herunter gesetzt

# Quelle

Das Repostory für Plugins liegt unter C:\Users\RobertThomitzek\Dropbox\Sourcen\esphome\tessera-plugins oder auf Github unter https://github.com/MaxGramser/homeassistant_espscreen

# Ziehlverzeichniss

Das Plugin sollte im Verzeichniss C:\Users\RobertThomitzek\Dropbox\Sourcen\esphome\tessera-shower-timer-card entstehen
Im Unterverzeichniss konzept werde ich meine schin verhandenen Informationen ablegen.

# Deteil Beschreibung

Im Projektverzeichniss unter konzept liegt eine Beispieldarstellung für das Plugin (Buttonvorlage.png bzw. Buttonvorlage.svg)
Das Plugin soll wie ein Button funktionieren der eine auszuwählende input_boolean entität Togelt. Bei mir ist das input_boolean.duschwassert. Unten Links soll der Status angezeigt werden. In Deutsch wäre das AN oder AUS. In der Mitte soll der aktuelle wert einer auszuwählenden Timer Entität amgezeigt werden. Bei mir ist das timer.duchwasser_warm. Unten recht soll ein auszuwählender Temperatursensor angezeigt werden. Bei mir ist das sensor.boiler_dhw_current_intern_temperature. Ist die input_boolean entität an, soll die Timer Restzeit in Rot angezeigt werden.

Die Bedienung sollte in Deutsch, Englisch, Portugiesisch und Niederländisch möglich sein.

Eine store.png sollte auch generiert werden. Hier sollte der Text in Englisch sein. Ausserdem im Readme erwähne werden, das eine KI beteiligt war.