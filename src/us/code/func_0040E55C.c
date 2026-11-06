#include "common.h"
#include "structs/vector.h"

/* Only what this reads and writes. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4F8];
    /* 0x4F8 */ CVector3 *target;
    /* 0x4FC */ u8 pad4FC[4];
    /* 0x500 */ s32 targetY;
    /* 0x504 */ u8 pad504[4];
    /* 0x508 */ s32 targetZ;
} Tracker;

s32 CScene__MarkGeneratorsSaved(s32);
extern u8 gScene[];

void BlindOne_PhaseMinionsInit(Tracker *arg0) {
    CVector3 *temp_v0;

    temp_v0 = arg0->target;
    arg0->targetY = (s32) temp_v0->y;
    arg0->targetZ = (s32) temp_v0->z;
    CScene__MarkGeneratorsSaved((s32) gScene);
}
