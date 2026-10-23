#include "common.h"

extern s32 D_800F7078[];
extern u8 D_801102B8[];
void func_00220260(void *table, void *actor, s32 arg);
void func_0026E110(void *actor, s32 flags, void *other);
void func_002365F8(void *list, void *actor);

void func_002662BC(u8 *actor) {
    s32 *table = D_800F7078;
    u8 *def = *(u8 **) (actor + 0x14);

    func_00220260(table, actor, 1);
    func_0026E110(actor, 0x80000, actor);
    func_002365F8(D_801102B8, actor);
    if (*table != 4) {
        *(s32 *) (actor + 0xD4) |= 0x8000000;
        if (*(s32 *) (def + 0x14) & 2) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s16 *) (*(u8 **) (actor + 0x14) + 0x28) == 0) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
    }
}
