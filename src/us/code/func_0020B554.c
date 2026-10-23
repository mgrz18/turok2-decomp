#include "common.h"

void func_0020B5B8(void);
extern s32 D_800AB8A0;
extern s32 D_800AB8A4;
extern s32 D_800AB8A8;
extern s32 D_800AB8AC;

void func_0020B554(void) {
    func_0020B5B8();
    if (D_800AB8A0) {
        D_800AB8A4 = 0x10000;
        D_800AB8A8 = 0xC4000000;
        D_800AB8AC = 0xC8000000;
    } else {
        D_800AB8A4 = 0;
        D_800AB8A8 = 0x0C080000;
        D_800AB8AC = 0x0C080000;
    }
}
