#include <Arduino.h>
#include "BuzzerMelody.h"

BuzzerMelody::BuzzerMelody(uint8_t pin, uint8_t channel)
{
  this->pin = pin;
  this->channel = channel;
  this->isWorking = ledcAttachChannel(pin, freq, resolution, channel);
}
bool BuzzerMelody::begin()
{
  if (isWorking)
  {
    this->isWorking = ledcWriteChannel(channel, 0);
  }
  this->chronoMelody = micros();
  this->chronoPause = micros();
  return isWorking;
}

uint32_t BuzzerMelody::dutyCycle()
{
  if (state == playing || state == music)
  {
    return (duty50 * volume / 100);
  }
  return 0;
}

bool BuzzerMelody::writeChannel()
{
  if (ledcWriteChannel(channel, 0))
  {
    if (ledcChangeFrequency(pin, freq, resolution))
    {
      this->changeVariable = false;
      return ledcWriteChannel(channel, dutyCycle());
    }
  }
  return false;
}

void BuzzerMelody::play()
{
  this->state = playing;
}

void BuzzerMelody::stop()
{
  this->state = stopped;
}
void BuzzerMelody::pause_micro(uint32_t timePause)
{
  if (state == stopped)
    return;
  this->timePaused = timePause;
  chronoPause = micros();
  this->state = pause;
}
void BuzzerMelody::pause_ms(uint32_t timePause)
{
  pause_micro(timePause * MILLI_SECOND);
}
void BuzzerMelody::pause_s(uint32_t timePause)
{
  pause_micro(timePause * SECOND);
}

void BuzzerMelody::refresh()
{
  if (isWorking == false)
    return;

  uint32_t now = micros();

  if (lastState != state || changeVariable)
  {
    lastState = state;
    writeChannel();
  }

  switch (state)
  {
  case (playing):
    break;
  case (stopped):
    break;
  case (pause):
    if (now - chronoPause >= timePaused && timePaused)
    {         // Compte le temps avant de remettre a play si on veux attendre, sinon on garde a pause
      play(); // Remet l'état à play pour continuer la note
    }
    break;
  case (music):
    melodyPlayer(now);
    break;
  }
}

void BuzzerMelody::setVolume(uint8_t volume)
{
  this->volume = constrain(volume, 0, 100);
  this->changeVariable = true;
}

void BuzzerMelody::writeResolution(uint32_t resolution)
{
  if (resolution >= 2 && resolution <= 16)
  {
    this->resolution = resolution;
    this->duty50 = ((1UL << resolution) / 2);
    this->changeVariable = true;
  }
  else
  {
    Serial.println("Error - Resolution has to be between 2-16");
  }
}
void BuzzerMelody::writeFrequency(uint32_t freq)
{
  this->freq = freq;
  this->changeVariable = true;
}
void BuzzerMelody::changeBpm(float bpm)
{
  if (bpm)
  {
    this->bpm = bpm;
    this->quarterNoteUs = MINUTE / bpm;
  }
  else
  {
    Serial.println("Error - bpm can't be 0");
  }
}
void BuzzerMelody::writeNote(NOTE_t note, uint8_t octave)
{
  if (note == MUTE)
  {
    freq = 0;
    changeVariable = true;
    return;
  }

  if (octave <= 8)
  {
    freq = (uint32_t)noteFrequencyBase[note] / (uint32_t)(1 << (8 - octave));
    changeVariable = true;
  }
  else
  {
    Serial.println("Error - Octave can't be higher then 8");
  }
}

void BuzzerMelody::playMelody(String *melodyPtr, uint8_t melodyLength)
{
  this->melody = melodyPtr;
  this->lengthMelody = melodyLength;
  this->state = music;
  this->stringStruct = true;
  this->cursorMelody = 0;
  this->chronoMelody = micros();
}
void BuzzerMelody::playMelody(keyNoteStruct *melodyPtr, uint8_t melodyLength)
{
  this->melodyStruct = melodyPtr;
  this->lengthMelody = melodyLength;
  this->state = music;
  this->stringStruct = false;
  this->cursorMelody = 0;
  this->chronoMelody = micros();
}
void BuzzerMelody::setContinuous(bool OnOff)
{
  this->continuous = OnOff;
}
void BuzzerMelody::melodyPlayer(uint32_t now)
{
  if (now - chronoMelody >= currentNoteDurationUs)
  {
    chronoMelody = now;
    if (cursorMelody < lengthMelody)
    {
      if (stringStruct)
      {
        decodeNoteString();
      }
      else
      {
        decodeNoteStruct();
      }
      cursorMelody++;
    }
    else
    {
      cursorMelody = 0;
      if (!continuous)
      {
        stop();
      }
    }
  }
}
void BuzzerMelody::decodeNoteStruct()
{
  tableNoteStruct = melodyStruct[cursorMelody];
  int noteCursor = 0;

  switch (tableNoteStruct.key)
  {
  case (doubleCroche):
    noteFactor = 0.25;
    break;
  case (croche):
    noteFactor = 0.5;
    break;
  case (noir):
    noteFactor = 1;
    break;
  case (blanche):
    noteFactor = 2;
    break;
  case (ronde):
    noteFactor = 4;
    break;
  default:
    Serial.print("Error - ");
    Serial.print(tableNoteStruct.key);
    Serial.print(" - Not a valid key {cc,c,n,b,r} - Position in the table : ");
    Serial.println(cursorMelody);
    return;
  }
  if (tableNoteStruct.dotted > 0)
  {
    noteFactor = 2 - pow(0.5, tableNoteStruct.dotted);
  }

  // Changing frequency and check ocateve <= 8
  currentNoteDurationUs = quarterNoteUs * noteFactor;
  writeNote(tableNoteStruct.note, tableNoteStruct.octave);
}
void BuzzerMelody::decodeNoteString()
{
  tableNoteString = melody[cursorMelody];
  int len = tableNoteString.length();
  int i = 0;

  if (len < 2)
  {
    Serial.println("Error - Empty or invalid note string");
    return;
  }

  // --- Duration key ---
  char k = tableNoteString[i++];

  if (k == 'c' && i < len && tableNoteString[i] == 'c')
  {
    noteFactor = 0.25f;
    i++;
  }
  else if (k == 'c')
    noteFactor = 0.5f;
  else if (k == 'n')
    noteFactor = 1.0f;
  else if (k == 'b')
    noteFactor = 2.0f;
  else if (k == 'r')
    noteFactor = 4.0f;
  else
  {
    Serial.print("Error - Invalid key: ");
    Serial.println(k);
    return;
  }

  // --- Dotted ---
  if (i < len && tableNoteString[i] == '.')
  {
    noteFactor *= 1.5f;
    i++;
  }

  if (i >= len)
  {
    Serial.println("Error - Missing note");
    return;
  }

  // --- Note ---
  NOTE_t note;

  switch (tableNoteString[i])
  {
  case 'A':
    note = A;
    break;
  case 'B':
    note = B;
    break;
  case 'C':
    note = C;
    break;
  case 'D':
    note = D;
    break;
  case 'E':
    note = E;
    break;
  case 'F':
    note = F;
    break;
  case 'G':
    note = G;
    break;
  case 'P':
    note = MUTE;
    break;

  default:
    Serial.print("Error - Invalid note: ");
    Serial.println(tableNoteString[i]);
    return;
  }

  i++;

  // --- Sharp / Flat ---
  if (i < len)
  {
    if (tableNoteString[i] == '#')
    {
      if (note < GsAf)
        note = (NOTE_t)(note + 1);
      i++;
    }
    else if (tableNoteString[i] == 'b')
    {
      if (note == A)
        note = GsAf;
      else
        note = (NOTE_t)(note - 1);
      i++;
    }
  }

  // --- Octave ---
  if (i >= len)
  {
    Serial.println("Error - Missing octave");
    return;
  }

  int octave = tableNoteString.substring(i).toInt();

  if (octave < 0 || octave > 8)
  {
    Serial.print("Error - Invalid octave: ");
    Serial.println(octave);
    return;
  }

  currentNoteDurationUs = quarterNoteUs * noteFactor;
  writeNote(note, octave);
}

uint32_t BuzzerMelody::readFreq()
{
  return ledcReadFreq(pin);
}

uint32_t BuzzerMelody::readDuty()
{
  return ledcRead(pin);
}

float BuzzerMelody::readMaxFreq()
{
  return 80e6 / (1 << resolution);
}