#include "common.h"
#include "structs/game_object.h"

/* Just the AI callbacks this sets. */
typedef struct {
    /* 0x000 */ u8 pad000[0x234];
    /* 0x234 */ void *onDraw;
    /* 0x238 */ u8 pad238[0x10];
    /* 0x248 */ void *onUpdate;
} Callbacks;

extern s32 func_00225F6C(void *world, s32 key0, s32 key1, s32 key2, Callbacks **out, s32 max);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void func_00407DC0();
extern void func_00403AD4();

extern u8 D_800F7078[];

void func_004071DC(CGameObjectInstance *obj, CAI *ai) {
    Callbacks *found[10];
    Callbacks *item;
    s32 n;
    s32 i;

    n = func_00225F6C(D_800F7078, 0x453, 9, -1, found, 10);
    for (i = 0; i < n; i++) {
        item = found[i];
        item->onDraw = func_00407DC0;
        item->onUpdate = func_00403AD4;
    }
    func_00243414(obj, ai, 0x1C);
}
