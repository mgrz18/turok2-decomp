#include "common.h"
#include "structs/vector.h"
#include "structs/boss.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* Only what this reads and writes. */
#ifdef TARGET_PORT
typedef struct Tracker {
/* the view at the host offsets of BlindOneState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Tracker(m) __builtin_offsetof(BlindOneState, m)
    union {
        struct { u8 _hp_target[T2_HOFF_Tracker(script)]; CVector3 *target; }; /* N64 0x4F8 */
        struct { u8 _hp_targetY[T2_HOFF_Tracker(toSpawn)]; s32 targetY; }; /* N64 0x500 */
        struct { u8 _hp_targetZ[T2_HOFF_Tracker(maxMinions)]; s32 targetZ; }; /* N64 0x508 */
    };
} Tracker;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x4F8];
    /* 0x4F8 */ CVector3 *target;
    /* 0x4FC */ u8 pad4FC[4];
    /* 0x500 */ s32 targetY;
    /* 0x504 */ u8 pad504[4];
    /* 0x508 */ s32 targetZ;
} Tracker;
#endif


void BlindOne_PhaseMinionsInit(Tracker *arg0) {
    CVector3 *temp_v0;

    temp_v0 = arg0->target;
    arg0->targetY = (s32) temp_v0->y;
    arg0->targetZ = (s32) temp_v0->z;
    CScene__MarkGeneratorsSaved(&gScene);
}
