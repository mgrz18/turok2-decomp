#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} WeaponDef;

/* The owning player: its current weapon. */
typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x118];
    /* 0x118 */ f32 unk118;
} WeaponState;

extern f32 D_800A7294;
extern WeaponDef *D_800AF618[];        /* per weapon */

void func_00258C20(CGameObjectInstance *arg0, WeaponState *arg1) {
    arg1->unk118 = (f32) (D_800AF618[((Player *) arg0->m_AI.m_pParent)->weapon]->unk18 * D_800A7294);
}
