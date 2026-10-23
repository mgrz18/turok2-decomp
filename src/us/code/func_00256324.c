#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + (off)))

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ CVector3 pos;
    /* 0x40 */ u8 pad40[0x54 - 0x40];
    /* 0x54 */ u16 flags;
} Region;

extern Region *func_00268A2C(CGameObjectInstance *actor);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_0020EAA0(CVector3 *v);
extern s32 func_00275624(s32 sound);

void func_00256324(CPlayer *player, CGameObjectInstance *actor) {
    CVector3 pos;
    CVector3 delta;
    Region *region = func_00268A2C(actor);

    P_S32(player, 0xB98) = 0;
    *(Region **)((u8 *)player + 0xBA0) = region;
    P_F32(player, 0xBA4) = actor->header.m_vPos.x;
    P_F32(player, 0xBA8) = actor->header.m_vPos.y;
    P_F32(player, 0xBAC) = actor->header.m_vPos.z;
    pos.x = region->pos.x;
    pos.y = region->pos.y;
    pos.z = region->pos.z;
    func_0020EF2C(&delta, &pos, &P_VEC(player, 0xBA4));
    delta.y = 0.0f;
    P_F32(player, 0xB9C) = func_0020EAA0(&delta);
    if (region == NULL || !(region->flags & 0x10)) {
        func_00275624(0x1450);
    }
}
