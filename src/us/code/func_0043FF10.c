#include "common.h"
#include "structs/boss.h"

extern s32 D_00430F28;
extern s32 D_00430FB4;
extern s32 D_00431004;
extern s32 D_004310B8;
extern s32 D_00431158;
extern s32 D_00431220;
void func_0043FF10(s32 arg0) {
    if (arg0 == 2) {
        if (D_8012F608.stage == 1) {
            D_8012F608.script.steps = &D_00430FB4;
        }
        if (D_8012F608.stage == arg0) {
            D_8012F608.script.steps = &D_004310B8;
        }
        if (D_8012F608.stage == 3) {
            D_8012F608.script.steps = &D_00431220;
        }
        D_8012F608.script.cursor = -1;
        D_8012F608.script.current = -1;
    }
    if (arg0 == 1) {
        if (D_8012F608.stage == arg0) {
            D_8012F608.script.steps = &D_00430F28;
        }
        if (D_8012F608.stage == 2) {
            D_8012F608.script.steps = &D_00431004;
        }
        if (D_8012F608.stage == 3) {
            D_8012F608.script.steps = &D_00431158;
        }
        D_8012F608.script.cursor = -1;
        D_8012F608.script.current = -1;
    }
}
