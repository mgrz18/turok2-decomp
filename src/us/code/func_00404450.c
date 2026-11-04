#include "common.h"
#include "structs/boss.h"

void func_00243414(s32 a0, s32 a1, s32 a2);
void func_00403C14(s32 mode);

void func_00404450(s32 a0, s32 a1) {
    s32 count;
    s32 i;
    s32 bit;

    count = 0;
    D_8012F608.bonus = 0;
    D_8012F608.total = 0;
    for (i = 0; i < 8; i++) {
        bit = 1 << i;
        if (D_8012F608.partsDone & bit) {
            D_8012F608.bonus += D_8012F608.values[1][i];
            if (D_8012F608.values[0][i] > 0) {
                D_8012F608.total += D_8012F608.values[0][i];
                count++;
            }
        }
    }
    if (D_8012F608.partActive == 0) {
        if (D_8012F608.stage == 3) {
            if (count == 0) {
                func_00243414(a0, a1, 0x1D);
            }
        } else if (count == 0) {
            D_8012F608.stage++;
            if (D_8012F608.stage >= 2) {
                D_8012F608.bossMode = 3;
                D_8012F608.bossState = 3;
                D_8012F608.armed = 0;
                D_8012F608.scriptOn = 0;
                D_8012F608.dest.x = D_8012F608.home.x;
                D_8012F608.dest.y = D_8012F608.home.y;
                D_8012F608.dest.z = D_8012F608.home.z;
                func_00243414(a0, a1, 0x12);
            }
            func_00403C14(D_8012F608.stage);
        }
    }
}
