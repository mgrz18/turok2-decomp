/* t2audio.c: whether the game's audio world is up on the host.
 *
 * Sound__Initialize is a host stub until the sound bank is converted (t2boot.c, docs/port/
 * AUDIO.md): gAudioWorld then has no bank and its pools are empty. The game's Sound__* entry
 * points return early on the host while T2Port_AudioReady() is false (each says so under
 * TARGET_PORT), so the rest of the game runs silent. Once the bank is loaded they run as on the
 * N64. */
#include "common.h"
#include "structs/sound.h"
#include "globals/audio.h"

int T2Port_AudioReady(void) {
    return gAudioWorld.bank != NULL;
}
