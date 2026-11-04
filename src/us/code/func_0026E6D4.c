#include "common.h"
#include "structs/game_object.h"

/* An event record (see func_0026DFB0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 actor;       /* index into the level actors */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u16 music;
    /* 0x0A */ u16 level;       /* level the actor lives in */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ u8 message;
    /* 0x11 */ u8 kind;         /* callback table */
    /* 0x12 */ u8 handler;      /* entry in that table */
} Event;

s32 func_00265F90(CGameObjectInstance *, CAI *, s32); /* extern */
extern CGameObjectInstance *D_800F7158;            /* level actors */
extern s32 D_80110020;                              /* current level */

s32 func_0026E6D4(Event *arg0, s32 arg1) {
    CGameObjectInstance *temp_a1;

    if (arg0->level == D_80110020) {
        if (arg1 != 0) {
            temp_a1 = &D_800F7158[arg0->actor];
            if ((arg0->kind == 0xA) && (arg0->handler == 1)) {
                return func_00265F90(temp_a1, &temp_a1->m_AI, arg1);
            }
        }
        return 1;
    }
    return 1;
}
