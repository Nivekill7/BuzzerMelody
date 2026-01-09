BuzzerMelody is an Arduino library designed for the ESP32 that allows non-blocking playback of musical notes and melodies on a passive buzzer using the LEDC hardware timer.

The library provides low-level control over frequency, duty cycle, resolution, and tempo (BPM), while also offering a simple melody engine based on human-readable string notation (e.g. "nC4", "c.D#5").

Unlike tone()-based approaches, BuzzerMelody relies on the ESP32 LEDC peripheral, enabling accurate frequency generation without blocking delays. Melody playback is driven by a state machine and must be serviced continuously via the refresh() method inside the main loop.

This library is intended for embedded projects where precise timing, non-blocking execution, and extensibility are required, such as robotics, alarms, user interfaces, or interactive devices.