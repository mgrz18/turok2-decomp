// t2audiodev.h - the sound follows the system's default output device (round 19 h4,
// docs/port/MACOS.md section 21).
//
// LUS's CoreAudio player opens an AUHAL unit on the device that is the default when it starts and
// never looks again: plugging in headphones, picking AirPods or unplugging the device it plays on
// left the game silent (MACOS.md 20.3). t2audiodev registers a listener for
// kAudioHardwarePropertyDefaultOutputDevice; the listener (on a CoreAudio thread) only counts
// the change, and the game thread, which feeds the player, re-creates it at its next sound frame
// (T2AudioDev_Poll): the new player opens on the new default. Other hosts (SDL3's player follows
// the default device itself) compile empty functions.
#pragma once

#include <memory>

namespace Ship {
class Audio;
class Config;
}

// After the Ship::Audio component opened its player: registers the listener when the player is
// CoreAudio's. Returns true when the listener is in place.
// config: the player's config (re-creating the player must not change its Window.AudioBackend).
bool T2AudioDev_Install(const std::shared_ptr<Ship::Audio>& audio, const std::shared_ptr<Ship::Config>& config);
// Removes the listener (before the Audio component goes away).
void T2AudioDev_Uninstall();
// The game thread, before it hands the player a sound frame: re-creates the player when the
// default output changed since the last call. Returns true when it did.
bool T2AudioDev_Poll();
// --audio-device-change-test (headless): calls the listener as CoreAudio would for a change of the
// default output, without changing the system's device.
void T2AudioDev_SimulateChange();
// how many times the player was re-created, and how many changes were seen
int T2AudioDev_Reopened();
int T2AudioDev_Changes();
