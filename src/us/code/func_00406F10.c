#include "common.h"
#include "structs/boss.h"

extern s32 D_00431008;
extern s32 D_00431094;
extern s32 D_004310E4;
extern s32 D_00431198;
extern s32 D_00431238;
extern s32 D_00431300;

void Primagen_SetAttackScript(s32 arg0) {
    if (arg0 == 2) {
        if (D_8012F608.stage == 1) {
            D_8012F608.script.steps = &D_00431094;
        }
        if (D_8012F608.stage == arg0) {
            D_8012F608.script.steps = &D_00431198;
        }
        if (D_8012F608.stage == 3) {
            D_8012F608.script.steps = &D_00431300;
        }
        D_8012F608.script.cursor = -1;
        D_8012F608.script.current = -1;
    }
    if (arg0 == 1) {
        if (D_8012F608.stage == arg0) {
            D_8012F608.script.steps = &D_00431008;
        }
        if (D_8012F608.stage == 2) {
            D_8012F608.script.steps = &D_004310E4;
        }
        if (D_8012F608.stage == 3) {
            D_8012F608.script.steps = &D_00431238;
        }
        D_8012F608.script.cursor = -1;
        D_8012F608.script.current = -1;
    }
}
