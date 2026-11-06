#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

/* The record whose position at 0x4F8 gets copied, as integers, to 0x500 / 0x508. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4F8];
    /* 0x4F8 */ CVector3 *pos;
    /* 0x4FC */ u8 pad4FC[4];
    /* 0x500 */ s32 y;
    /* 0x504 */ u8 pad504[4];
    /* 0x508 */ s32 z;
} PosCopy;

s32 CScene__MarkGeneratorsSaved(s32);


void BlindOne_PhaseMinionsInit_2(PosCopy *arg0) {
    CVector3 *temp_v0;

    temp_v0 = arg0->pos;
    arg0->y = temp_v0->y;
    arg0->z = temp_v0->z;
    CScene__MarkGeneratorsSaved((s32) &gScene);
}
