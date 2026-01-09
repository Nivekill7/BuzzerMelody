#include "BuzzerMelody.h"

#include <Decodeur.h>

// Decoder instance using Serial as input source
Decodeur monDecodeur(&Serial);

#define buzzerPin 19
#define buzzerChannel 0

// Buzzer object using a specific pin and PWM channel
BuzzerMelody buzzer(buzzerPin, buzzerChannel);

// Melody format:
// (key duration, dotted note optional, note, sharp or flat optional, octave)
//
// Examples:
//  nC4   -> normal C, octave 4
//  n.B8  -> dotted B, octave 8
//  cc.B#8-> double croche, dotted, B sharp, octave 8
String melody[] = {
  "nC0",
  "bC1",
  "cC2",
  "ccC3",
  "nC4",
  "nC5",
  "nC6",
  "nC7",
  "nC8",
  "nB1",
  "nB2",
  "nB3",
  "nB5",
  "nB6",
  "nB7",
  "nB#8",
  "nBb8",
  "n.B8",
  "cc.B#8"
};

// Total number of notes in the melody array
const int melodynote = 19;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize the buzzer with a default BPM value
  buzzer.begin(100);

  Serial.println("Hello");
}

void loop() {
  // Update buzzer internal state (timing, melody playback, etc.)
  buzzer.refresh();

  // Update decoder state to check for new serial commands
  monDecodeur.refresh();

  // Process incoming command if available
  if (monDecodeur.isAvailable()) {
    switch (monDecodeur.getCommand()) {

      // Play current sound or melody
      case ('p'):
        buzzer.play();
        break;

      // Stop any sound or melody
      case ('s'):
        buzzer.stop();
        break;

      // Pause playback for a given duration in milliseconds
      case ('t'):
        buzzer.pause_ms((long)monDecodeur.getArg(0));
        break;

      // Set buzzer frequency directly
      case ('f'):
        buzzer.writeFreq(monDecodeur.getArg(0));
        break;

      // Change PWM resolution
      case ('r'):
        buzzer.writeResolution(monDecodeur.getArg(0));
        break;

      // Play the predefined melody
      case ('m'):
        buzzer.playMelody(melody, melodynote);
        break;

      // Play a single note with a given octave
      case ('k'):
        buzzer.writeNote((keyNote_t)monDecodeur.getArg(0), monDecodeur.getArg(1));
        break;

      // Print current frequency to the serial monitor
      case ('g'):
        Serial.println(buzzer.readFreq());
        break;

      // Print current duty cycle to the serial monitor
      case ('d'):
        Serial.println(buzzer.readDuty());
        break;

      // Change BPM (tempo) of the melody
      case ('b'):
        buzzer.changeBpm(monDecodeur.getArg(0));
        break;

      // Enable continuous melody playback
      case ('c'):
        buzzer.melody_continuous(true);
        break;

      // Enable single-play mode for the melody
      case ('o'):
        buzzer.melody_playOnce(true);
        break;
    }
  }

  // Allow background tasks (important on ESP32)
  yield();
}
