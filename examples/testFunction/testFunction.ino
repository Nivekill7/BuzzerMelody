/*
ESP32 board  
*/
#include <BuzzerMelody.h>

/*
INSTALL TGP DECODEUR v2.0.0
*/
#include <Decodeur.h>

/*
 =========================================================
 BuzzerMelody Library - Star Wars & Super Mario Bros
 =========================================================
 Mélodies incluses:
 - Star Wars Main Theme (STRING notation)
 - Super Mario Bros Theme (STRUCT notation)
 
 Commandes Serial @115200 baud:
 1 -> Star Wars (STRING)
 2 -> Super Mario Bros (STRUCT)
 s -> Stop
 p -> Pause 2s
 c -> Mode continu ON/OFF
 v X -> Volume (0-100)
 b X -> BPM
 f X -> Fréquence directe (Hz)
 x -> Arrêter le ton
*/

Decodeur decoder(&Serial);

// Pin of the buzzer
#define BUZZER_PIN 19
#define BUZZER_CHANNEL 0

BuzzerMelody buzzer(BUZZER_PIN, BUZZER_CHANNEL);

// =====================================================
// STAR WARS MAIN THEME - STRING Notation
// =====================================================
// Tempo: 108 BPM
// Format: "key[.]NOTE[#/b]OCTAVE"
// key: cc=double croche, c=croche, n=noir, b=blanche, r=ronde
// .: note pointée (dotted)
// NOTE: A-G, # ou b pour sharp/flat, P=pause
// =====================================================

String starWarsTheme[] = {
  // Introduction fanfare (mesure 1-4)
  "cD4", "cD4", "cD4",
  "nG4", "n.D5",

  // Mélodie principale (mesure 5-8)
  "nC5", "nB4", "nA4", "bG5", "cD5",
  "nC5", "nB4", "nA4", "bG5", "cD5",
  "nC5", "nB4", "nC5", "nA4",

  // Répétition thème (mesure 9-12)
  "cD4", "cD4", "cD4",
  "nG4", "n.D5",
  "nC5", "nB4", "nA4", "bG5", "cD5",

  // Section médiane (mesure 13-16)
  "nC5", "nB4", "nA4", "bG5", "cD5",
  "nC5", "nB4", "nC5", "nA4",

  // Finale (mesure 17-20)
  "n.D4", "n.D4", "nE4", "nC4", "nB3", "nA3",
  "nG3", "cD4", "nE4", "cC4", "cB3", "cA3",
  "nG3", "cD4", "nE4", "cC4", "cB3", "cC4",

  // Ending
  "rA3",
  "nP", "nP"
};

uint8_t starWarsSize = sizeof(starWarsTheme) / sizeof(starWarsTheme[0]);

// =====================================================
// SUPER MARIO BROS THEME - STRUCT Notation
// =====================================================
// Tempo: 200 BPM (rapide et énergique)
// Structure plus précise avec enum
// =====================================================

keyNoteStruct marioTheme[] = {
  // Introduction iconique (mesure 1-2)
  { croche, 0, E, 5 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, C, 5 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, G, 5 },
  { noir, 0, MUTE, 0 },
  { croche, 0, G, 4 },
  { noir, 0, MUTE, 0 },

  // Mélodie principale (mesure 3-6)
  { noir, 0, C, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, G, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, A, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, B, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, AsBf, 4 },
  { croche, 0, A, 4 },
  { croche, 0, MUTE, 0 },

  // Section rythmique (mesure 7-10)
  { doubleCroche, 0, G, 4 },
  { doubleCroche, 0, E, 5 },
  { doubleCroche, 0, G, 5 },
  { croche, 0, A, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, F, 5 },
  { croche, 0, G, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, C, 5 },
  { croche, 0, D, 5 },
  { croche, 0, B, 4 },
  { croche, 0, MUTE, 0 },

  // Répétition mélodie (mesure 11-14)
  { noir, 0, C, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, G, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, A, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, B, 4 },
  { croche, 0, MUTE, 0 },
  { croche, 0, AsBf, 4 },
  { croche, 0, A, 4 },
  { croche, 0, MUTE, 0 },

  // Section finale (mesure 15-18)
  { doubleCroche, 0, G, 4 },
  { doubleCroche, 0, E, 5 },
  { doubleCroche, 0, G, 5 },
  { croche, 0, A, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, F, 5 },
  { croche, 0, G, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, C, 5 },
  { croche, 0, D, 5 },
  { croche, 0, B, 4 },
  { noir, 0, MUTE, 0 },

  // Section descendante (mesure 19-22)
  { croche, 0, G, 5 },
  { croche, 0, FsGf, 5 },
  { croche, 0, F, 5 },
  { croche, 0, DsEf, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, GsAf, 4 },
  { croche, 0, A, 4 },
  { croche, 0, C, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, A, 4 },
  { croche, 0, C, 5 },
  { croche, 0, D, 5 },

  // Ending
  { croche, 0, G, 5 },
  { croche, 0, FsGf, 5 },
  { croche, 0, F, 5 },
  { croche, 0, DsEf, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, E, 5 },
  { croche, 0, MUTE, 0 },
  { croche, 0, C, 6 },
  { croche, 0, MUTE, 0 },
  { croche, 0, C, 6 },
  { croche, 0, C, 6 },
  { noir, 0, MUTE, 0 },
  { noir, 0, MUTE, 0 }
};

uint8_t marioSize = sizeof(marioTheme) / sizeof(marioTheme[0]);

// =====================================================
// VARIABLES GLOBALES
// =====================================================
bool continuousFlag = false;

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  // Initialisation du buzzer
  if (!buzzer.begin()) {
    Serial.println("❌ ERREUR: Échec d'initialisation LEDC");
    while (1) { delay(1000); }
  }

  // Configuration par défaut
  buzzer.setVolume(40);   // Volume modéré
  buzzer.changeBpm(120);  // Tempo par défaut

  // Affichage du menu
  Serial.println();
  Serial.println("╔════════════════════════════════════════════════╗");
  Serial.println("║  🎵 BuzzerMelody - Test Console 🎵              ║");
  Serial.println("╠════════════════════════════════════════════════╣");
  Serial.println("║  MÉLODIES:                                     ║");
  Serial.println("║  1 → Star Wars Theme (STRING)    [108 BPM]     ║");
  Serial.println("║  2 → Super Mario Bros (STRUCT)   [200 BPM]     ║");
  Serial.println("║                                                ║");
  Serial.println("║  CONTRÔLES:                                    ║");
  Serial.println("║  s → Stop la mélodie                           ║");
  Serial.println("║  p → Pause 2 secondes                          ║");
  Serial.println("║  c → Mode continu ON/OFF                       ║");
  Serial.println("║                                                ║");
  Serial.println("║  PARAMÈTRES:                                   ║");
  Serial.println("║  v X → Volume (0-100)                          ║");
  Serial.println("║  b X → BPM (tempo)                             ║");
  Serial.println("║  f X → Fréquence directe (Hz)                  ║");
  Serial.println("║  x → Arrêter le ton direct                     ║");
  Serial.println("╚════════════════════════════════════════════════╝");
  Serial.println();
  Serial.println("✅ Système prêt. Entrez une commande...");
  Serial.println();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  // Rafraîchir le buzzer (gestion de la mélodie)
  buzzer.refresh();

  // Rafraîchir le décodeur (lecture des commandes)
  decoder.refresh();


  // Traiter les commandes
  if (decoder.isAvailable()) {

    char cmd = decoder.getCommand();

    if (cmd == '1') {
      buzzer.stop();
      buzzer.changeBpm(108);
      buzzer.playMelody(starWarsTheme, starWarsSize);

      Serial.println("Star Wars Theme (STRING) - 108 BPM");
      Serial.print("Duree approx: ");
      Serial.print(starWarsSize * 0.55);
      Serial.println(" s");
    }

    else if (cmd == '2') {
      buzzer.stop();
      buzzer.changeBpm(200);
      buzzer.playMelody(marioTheme, marioSize);

      Serial.println("Mario Bros Theme (STRUCT) - 200 BPM");
      Serial.print("Duree approx: ");
      Serial.print(marioSize * 0.30);
      Serial.println(" s");
    }

    else if (cmd == 's') {
      buzzer.stop();
      Serial.println("Buzzer stopped");
    }

    else if (cmd == 't') {
      int pauseMs = decoder.getArg(0);
      pauseMs = constrain(pauseMs, 0, 10000);
      buzzer.pause_ms(pauseMs);

      Serial.print("Pause ");
      Serial.print(pauseMs);
      Serial.println(" ms");
    }

    else if (cmd == 'p') {
      buzzer.play();
      Serial.println("Play ");
    }

    else if (cmd == 'c') {
      continuousFlag = !continuousFlag;
      buzzer.setContinuous(continuousFlag);

      Serial.print("Continuous mode: ");
      Serial.println(continuousFlag ? "ON" : "OFF");
    }

    else if (cmd == 'v') {
      int vol = decoder.getArg(0);
      vol = constrain(vol, 0, 100);
      buzzer.setVolume(vol);

      Serial.print("Volume: ");
      Serial.print(vol);
      Serial.println("%");
    }

    else if (cmd == 'b') {
      int bpm = decoder.getArg(0);
      bpm = constrain(bpm, 40, 300);
      buzzer.changeBpm(bpm);

      Serial.print("Tempo: ");
      Serial.print(bpm);
      Serial.println(" BPM");
    }

    else if (cmd == 'f') {
      int freq = decoder.getArg(0);
      freq = constrain(freq, 20, 20000);
      buzzer.writeFrequency(freq);

      Serial.print("Tone direct: ");
      Serial.print(freq);
      Serial.println(" Hz");
    }

    else if (cmd == 'x') {
      buzzer.stop();  // coupe uniquement le tone direct
      Serial.println("Tone stopped");
    }

    else {
      Serial.println("Not a command");
    }
  }


  yield();  // Permet au watchdog de respirer
}