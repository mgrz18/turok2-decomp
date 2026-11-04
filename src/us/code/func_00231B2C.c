#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct {
    u8 pad0[0x18];
    /* 0x018 */ CVector3 origin;
    u8 pad24[0x1C];
    /* 0x040 */ u32 flags;
    u8 pad44[0xB4];
    /* 0x0F8 */ u32 *parentFlags;
    u8 padFC[0x10];
    /* 0x10C */ void *parent;
    /* 0x110 */ void *slot;
    /* 0x114 */ s32 a;
} Actor;

extern u8 D_800F8928[];
extern CCollisionResults D_800F1CE0;  /* the last collision test: the spawn lands on its floor hit */
extern CVector3 D_800F1DB8;
void func_0020E810(CQuaternion *out, CVector3 *dir);
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);

void func_00231B2C(Actor *actor, s32 kind) {
    CQuaternion rot;
    CVector3 origin;

    if (*actor->parentFlags & 0x10) {
        origin = D_800F1DB8;
    } else {
        origin = actor->origin;
    }
    func_0020E810(&rot, &origin);
    func_0022F350(D_800F8928, actor, actor->parent, actor->slot, actor->a, kind, origin, rot,
                  D_800F1CE0.m_vFloorPos, (s32)D_800F1CE0.m_pFloorRegion, -2, (actor->flags & 0x200006) | 1);
}
