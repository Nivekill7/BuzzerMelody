#ifndef BUZZERMELODY_H
#define BUZZERMELODY_H
#include <Arduino.h>

typedef enum {
  KEY_A,
  KEY_AsBf,
  KEY_B,
  KEY_C,
  KEY_CsDf,
  KEY_D,
  KEY_DsEf,
  KEY_E,
  KEY_F,
  KEY_FsGf,
  KEY_G,
  KEY_GsAf,
  MAX_KEY
} keyNote_t;

const uint16_t noteFrequencyBase[MAX_KEY] = {
  //A,  AsBf,  B,    C,   CsDf,  D,   DsEf,  E,    F,   FsGf,  G,   GsAf
  7040, 7459, 7902, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645
};

class BuzzerMelody {
public:

  BuzzerMelody(int pin, int channel);
  bool begin(uint16_t bpm);
  void play();
  void stop();
  void pause_micro(long timePause);
  void pause_ms(long timePause);
  void pause_s(long timePause);
  void refresh();
  void writeResolution(uint32_t resolution);
  void writeFreq(uint32_t freq);
  void writeNote(keyNote_t note, uint8_t octave);
  void changeBpm(float bpm);
  void playMelody(String* melodyPtr, uint8_t lenght);
  void melody_continuous(bool OnOff);
  uint32_t readFreq();
  uint32_t readDuty();
  float readMaxFreq();

private:
  uint16_t dutyCycle();
  bool writeChannel();
  void melodyPlayer(uint32_t now);
  void decodeNote();

  enum {
    stopped,
    playing,
    pause,
    music
  };

  int pin, state = 0, lastState = 0, channel = 0;
  uint32_t resolution = 12, duty50 = ((1UL << resolution) / 2), freq = 440, chronoPause = 0, timePaused = 0;
  bool isWorking = false, changeVariable = false;

  //Melody
  const int MINUTE = 1e7, SECOND = 1e6, MILLI_SECONDE = 1e3;
  uint32_t chronoMelody = 0;
  uint16_t startMelody, cursorMelody = 0, lenghtMelody = 0, noteFrequency = 0;
  float quarterNoteMs = 600000.0, noteFactor = 1.0, bpm = 100;
  bool continuous = false;
  String tableNote;
  String* melody = nullptr;  // pointeur vers le tableau
};
#endif