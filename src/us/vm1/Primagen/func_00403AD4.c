#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* the bone-posing context at its host layout (round 10 y2, as x2's MotherBoss_PartEffects): the
 * bone at T2PoseContext.bone, not N64 0x90; the matrix (a0) pointer-wide, not an s32 */
typedef struct {
    u8 pad0[__builtin_offsetof(T2PoseContext, bone)];
    s32 id;
} Obj00403AD4;
_Static_assert(__builtin_offsetof(Obj00403AD4, id) == __builtin_offsetof(T2PoseContext, bone), "Obj00403AD4");
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 id;
} Obj00403AD4;
#endif

void CMtxF__ScaleAxes(sptr a0, f32 r, f32 g, f32 b);

void Primagen_Effects(sptr a0, Obj00403AD4 *obj) { /* a0 pointer-wide (sptr: s32 on the N64) */
    f32 level = 1.0f - gLevelScript.partTimer * 0.011111111f;

    switch (obj->id) {
        case 0x6C:
            if (gLevelScript.partActive && gLevelScript.part == 0) {
                CMtxF__ScaleAxes(a0, level, level, level);
            }
            break;
        case 0x68:
            if (gLevelScript.partActive && gLevelScript.part == 2) {
                CMtxF__ScaleAxes(a0, level, level, level);
            }
            break;
        case 0x33:
            if (gLevelScript.partActive && gLevelScript.part == 3) {
                CMtxF__ScaleAxes(a0, level, level, level);
            }
            break;
        case 0x29:
            if (gLevelScript.partActive && gLevelScript.part == 6) {
                CMtxF__ScaleAxes(a0, level, level, level);
            }
            break;
        case 0x1C:
            if (gLevelScript.partActive && gLevelScript.part == 4) {
                CMtxF__ScaleAxes(a0, level, level, level);
            }
            break;
        case 0x2E:
            if (gLevelScript.partActive && gLevelScript.part == 5) {
                CMtxF__ScaleAxes(a0, level, level, level);
            }
            break;
    }
}
