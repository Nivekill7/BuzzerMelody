#include <Arduino.h>
#include "BuzzerMelody.h"

BuzzerMelody::BuzzerMelody(int pin, int channel) {
  this->pin = pin;
  this->channel = channel;
  this->isWorking = ledcAttachChannel(pin, freq, resolution, channel);
}
bool BuzzerMelody::begin(uint16_t bpm) {
  if (isWorking) {
    this->isWorking = ledcWriteChannel(channel, 0);
  }
  this->bpm = bpm;
  this->chronoMelody = micros();
  this->chronoPause = micros();
  return isWorking;
}

uint16_t BuzzerMelody::dutyCycle() {
  if (state == stopped || state == pause) {
    return 0;
  } else if (state == playing || state == music) {
    return duty50;
  }
  return 0;
}

bool BuzzerMelody::writeChannel() {
  if (ledcWriteChannel(channel, 0)) {
    if (!ledcChangeFrequency(pin, freq, resolution)) {
      return false;
    }
  }
  this->changeVariable = false;
  return ledcWriteChannel(channel, dutyCycle());
}

void BuzzerMelody::play() {
  this->state = playing;
}

void BuzzerMelody::stop() {
  this->state = stopped;
}
void BuzzerMelody::pause_ms(long timePause) {
  pause_micro(timePause * 1e3);
}
void BuzzerMelody::pause_s(long timePause) {
  pause_micro(timePause * 1e6);
}
void BuzzerMelody::pause_micro(long timePause) {
  if (state == stopped)
    return;
  this->timePaused = timePause;
  chronoPause = micros();
  this->state = pause;
}

void BuzzerMelody::refresh() {
  if (isWorking == false)
    return;

  uint32_t now = micros();

  if (lastState != state || changeVariable) {
    lastState = state;
    writeChannel();
  }

  switch (state) {
    case (playing):
      break;
    case (stopped):
      break;
    case (pause):
      if (now - chronoPause >= timePaused && timePaused) {  // Compte le temps avant de remettre a play si on veux attendre, sinon on garde a pause
        play();                                             // Remet l'état à play pour continuer la note
      }
      break;
    case (music):
      melodyPlayer(now);
      break;
  }
}

void BuzzerMelody::writeResolution(uint32_t resolution) {
  if (resolution >= 2 && resolution <= 16) {
    this->resolution = resolution;
    this->duty50 = ((1UL << resolution) / 2);
    this->changeVariable = true;
  } else {
    Serial.println("Error - Resolution has to be between 2-16");
  }
}
void BuzzerMelody::writeFreq(uint32_t freq) {
  this->freq = freq;
  this->changeVariable = true;
}
void BuzzerMelody::changeBpm(float bpm) {
  if (bpm) {
    this->bpm = bpm;
    this->quarterNoteMs = MINUTE / bpm;
  } else {
    Serial.println("Error - bpm can't be 0");
  }
}
void BuzzerMelody::writeNote(keyNote_t note, uint8_t octave) {
  if (octave < 8) {
    freq = (uint32_t)noteFrequencyBase[note] / (uint32_t)(1 << (8 - octave));
    changeVariable = true;
  } else {
    Serial.println("Error - Octave can't be higher then 8");
  }
}

void BuzzerMelody::playMelody(String *melodyPtr, uint8_t melodyLenght) {
  this->melody = melodyPtr;
  this->lenghtMelody = melodyLenght;
  this->state = music;
  this->cursorMelody = 0;
}
void BuzzerMelody::melody_continuous(bool OnOff) {
  this->continuous = OnOff;
}
void BuzzerMelody::melodyPlayer(uint32_t now) {
  if (now - chronoMelody >= quarterNoteMs * noteFactor) {
    chronoMelody = now;
    if (cursorMelody < lenghtMelody) {
      decodeNote();
      cursorMelody++;
    } else {
      cursorMelody = 0;
      if (!continuous) {
        stop();
      }
    }
  }
}
void BuzzerMelody::decodeNote() {

  tableNote = melody[cursorMelody];
  int noteCursor = 0;

  int tableLength = tableNote.length();
  if (tableLength < 3) {
    Serial.print("Error - ");
    Serial.print(tableNote);
    Serial.print(" - Missing arguments {key,note,octave}");
    stop();
    return;
  }

  // Setting time of note
  switch (tableNote[noteCursor]) {
    case ('c'):  // doubleCroche
      noteCursor++;
      if (noteCursor >= tableLength) {
        return;
      } else if (tableNote[noteCursor] == 'c') {
        noteFactor = 0.25;
        noteCursor = 2;
      } else {  // croche
        noteFactor = 0.5;
        noteCursor = 1;
      }
      break;
    case ('n'):  // noir
      noteFactor = 1;
      noteCursor = 1;
      break;
    case ('b'):  // blanche
      noteFactor = 2;
      noteCursor = 1;
      break;
    case ('r'):  // ronde
      noteFactor = 4;
      noteCursor = 1;
      break;
    default:
      Serial.printf("Error - %c - Not a valid key {cc,c,n,b,r}\n", tableNote[noteCursor]);
      stop();
      return;
      break;
  }

  // Dots
  if (tableNote[noteCursor] == '.') {
    noteFactor *= 1.5;
    noteCursor++;
  }

  // Notes
  if (noteCursor >= tableLength) {
    Serial.print("Error - ");
    Serial.print("Missing argument at this position in the table : ");
    Serial.println(cursorMelody);
    stop();
    return;
  }
  char note = tableNote[noteCursor];
  noteCursor++;

  // Setting frequency
  switch (note) {
    case ('A'):
      noteFrequency = KEY_A;
      break;
    case ('B'):
      noteFrequency = KEY_B;
      break;
    case ('C'):
      noteFrequency = KEY_C;
      break;
    case ('D'):
      noteFrequency = KEY_D;
      break;
    case ('E'):
      noteFrequency = KEY_E;
      break;
    case ('F'):
      noteFrequency = KEY_F;
      break;
    case ('G'):
      noteFrequency = KEY_G;
      break;
    default:
      Serial.print("Error - ");
      Serial.print(note);
      Serial.println(" - Not a valid note {A,B,C,D,E,F,G}");
      stop();
      return;
      break;
  }

  // Sharp of Flat
  if (noteCursor >= tableLength) {
    Serial.print("Error - ");
    Serial.print("Missing argument at this position in the table : ");
    Serial.println(cursorMelody);
    stop();
    return;
  }
  char alteration = tableNote[noteCursor];

  if (alteration == '#') {
    noteCursor++;
    noteFrequency++;
  } else if (alteration == 'b') {
    noteCursor++;
    if (noteFrequency == KEY_A) {
      noteFrequency = KEY_GsAf;
    } else {
      noteFrequency--;
    }
  }

  // Setting Octave
  if (noteCursor >= tableLength) {
    Serial.print("Error - ");
    Serial.print("Missing argument at this position in the table : ");
    Serial.println(cursorMelody);
    stop();
    return;
  }
  uint8_t octave = tableNote.substring(noteCursor).toInt();
  if (octave > 8) {
    Serial.printf("Error - i% - Not a valid octave {0-8}\n", octave);
    stop();
    return;
  }

  // Changing frequency
  this->freq = (uint16_t)noteFrequencyBase[noteFrequency] / (uint16_t)(1 << (8 - octave));
  this->changeVariable = true;
}

uint32_t BuzzerMelody::readFreq() {
  return ledcReadFreq(pin);
}

uint32_t BuzzerMelody::readDuty() {
  return ledcRead(pin);
}

float BuzzerMelody::readMaxFreq() {
  return 80e6 / (1 << resolution);
}