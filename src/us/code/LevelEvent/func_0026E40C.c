#include "common.h"

s32 Music_SetTrack(u16);

/* The mode parameter this AI callback is handed: only the field it reads. */
typedef struct AIModeParam {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 value;
} AIModeParam;                         /* extern */

void Event_SetMusicRemote(AIModeParam *arg0) {
    Music_SetTrack(arg0->value);
}
