/*  Embedis - Embedded Dictionary Server
    Copyright (C) 2015 PatternAgents, LLC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Embedis.h>
#include <EEPROM.h>

// Embedis will run on the Serial port. Use the Arduino
// serial monitor and send "COMMANDS" to get started.
// Make sure "No line ending" is -not- selected. All others work.
Embedis embedis(Serial);

// If E2END isn't defined you can manually set this.
const size_t EEPROM_SIZE = E2END + 1;

void setup() 
{
    Serial.begin(115200);
    
    // Add a key-value store.
    Embedis::dictionary( "EEPROM",
        EEPROM_SIZE,
        [](size_t pos) -> char { return EEPROM.read(pos); },
        [](size_t pos, char value) { EEPROM.write(pos, value); }
    );
    
    // Add pinMode command to mirror Arduino's
    Embedis::command( F("pinMode"), [](Embedis* e) {
        if (e->argc != 3) return e->response(Embedis::ARGS_ERROR);
        int pin = String(e->argv[1]).toInt();
        String argv3(e->argv[2]);
        argv3.toUpperCase();
        int mode;
        if (argv3 == "INPUT") mode = INPUT;
        else if (argv3 == "OUTPUT") mode = OUTPUT;
        else if (argv3 == "INPUT_PULLUP") mode = INPUT_PULLUP;
        else return e->response(Embedis::ARGS_ERROR);
        pinMode(pin, mode);
        e->response(Embedis::OK);
    });

    // Add digitalWrite command to mirror Arduino's
    Embedis::command( F("digitalWrite"), [](Embedis* e) {
        if (e->argc != 3) return e->response(Embedis::ARGS_ERROR);
        int pin = String(e->argv[1]).toInt();
        String argv3(e->argv[2]);
        argv3.toUpperCase();
        int mode;
        if (argv3 == "HIGH") mode = HIGH;
        else if (argv3 == "LOW") mode = LOW;
        else mode = argv3.toInt();
        digitalWrite(pin, mode);
        e->response(Embedis::OK);
    });

    // Add digitalRead command to mirror Arduino's
    Embedis::command( F("digitalRead"), [](Embedis* e) {
        if (e->argc != 2) return e->response(Embedis::ARGS_ERROR);
        int pin = String(e->argv[1]).toInt();
        if (digitalRead(pin)) {
            e->response(F("HIGH"));
        } else {
            e->response(F("LOW"));
        }
    });

    // Add analogRead command to mirror Arduino's
    Embedis::command( F("analogRead"), [](Embedis* e) {
        if (e->argc != 2) return e->response(Embedis::ARGS_ERROR);
        int pin = String(e->argv[1]).toInt();
        e->response(':', analogRead(pin));
    });

}

void loop() 
{
    embedis.process();
}
