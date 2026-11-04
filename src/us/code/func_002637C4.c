#include "common.h"

/* An event record (see func_0026DFB0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 actor;       /* index into the level actors */
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ u16 level;       /* level the actor lives in */
} Event;

void func_002245BC(void *, u16, u16, s32);          /* extern */
extern u8 D_800F7078[];

void func_002637C4(Event *arg0) {
    func_002245BC(D_800F7078, arg0->level, arg0->actor, 0);
}
