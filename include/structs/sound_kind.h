#ifndef STRUCTS_SOUND_KIND_H
#define STRUCTS_SOUND_KIND_H

#include "common.h"

/* One row of sSoundKindVolumeTable: a sound kind (< 0x100, stored as a float)
 * and the volume scale SndVoice_InitVolume gives its voices. */
typedef struct SoundKindVolume {
    /* 0x0 */ f32 kind;
    /* 0x4 */ f32 scale;
} SoundKindVolume; /* size 0x8 */

#endif
