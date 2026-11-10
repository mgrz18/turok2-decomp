#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 id;
} Obj00403AD4;

void CMtxF__ScaleAxes(sptr a0, f32 r, f32 g, f32 b);

void Primagen_Effects(s32 a0, Obj00403AD4 *obj) {
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
