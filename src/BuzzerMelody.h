#ifndef BUZZERMELODY_H
#define BUZZERMELODY_H
#include <Arduino.h>

typedef enum
{
  A,
  AsBf,
  B,
  C,
  CsDf,
  D,
  DsEf,
  E,
  F,
  FsGf,
  G,
  GsAf,
  MUTE,
  MAX_NOTE
} NOTE_t;

typedef enum
{
  doubleCroche,
  croche,
  noir,
  blanche,
  ronde
} KEY_T;
struct keyNoteStruct
{
  KEY_T key;
  uint8_t dotted;
  NOTE_t note;
  uint8_t octave;
};

const uint16_t noteFrequencyBase[MAX_NOTE] = {
  //  A,  AsBf,  B,    C,   CsDf,  D,  DsEf,  E,    F,   FsGf,  G,   GsAf, MUTE
    7040, 7459, 7902, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 0};

class BuzzerMelody
{
public:
  BuzzerMelody(uint8_t pin, uint8_t channel);
  bool begin();
  void play();
  void stop();
  void pause_micro(uint32_t timePause);
  void pause_ms(uint32_t timePause);
  void pause_s(uint32_t timePause);
  void refresh();
  void setVolume(uint8_t volume);
  void writeResolution(uint32_t resolution);
  void writeFrequency(uint32_t freq);
  void writeNote(NOTE_t note, uint8_t octave);
  void changeBpm(float bpm);
  void playMelody(String *melodyPtr, uint8_t length);
  void playMelody(keyNoteStruct *melodyPtr, uint8_t length);
  void setContinuous(bool OnOff);
  uint32_t readFreq();
  uint32_t readDuty();
  float readMaxFreq();

private:
  uint32_t dutyCycle();
  bool writeChannel();
  void melodyPlayer(uint32_t now);
  void decodeNoteStruct();
  void decodeNoteString();

  enum
  {
    stopped,
    playing,
    pause,
    music
  };

  uint8_t pin, state = 0, lastState = 0, channel = 0, volume = 100;
  uint32_t resolution = 12, duty50 = ((1UL << resolution) / 2), freq = 440, chronoPause = 0, timePaused = 0;
  bool isWorking = false, changeVariable = false;

  // Melody
  const int MINUTE = 60e6, SECOND = 1e6, MILLI_SECOND = 1e3;
  uint32_t chronoMelody = 0;
  uint16_t cursorMelody = 0, lengthMelody = 0, noteFrequency = 0;
  float noteFactor = 1.0, bpm = 100, quarterNoteUs = MINUTE / bpm, currentNoteDurationUs = quarterNoteUs * noteFactor;
  bool continuous = false, stringStruct = false;
  String tableNoteString;
  String *melody = nullptr; // pointeur vers le tableau
  keyNoteStruct tableNoteStruct;
  keyNoteStruct *melodyStruct = nullptr; // pointeur vers le tableau
};
#endif