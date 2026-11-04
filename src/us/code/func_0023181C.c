#include "common.h"
#include "structs/game_object.h"

s32 func_0022F1C8(s32 *, void *);           /* extern */
s32 func_00232B40(void *);                      /* extern */
s16 *func_0025398C(s32, s32);                   /* extern */
extern s32 D_800F8928;

/* A drawn instance; 0xF8 points at its render info, 0x1D0 links it in the
 * world's draw list at 0xEC14. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 unk14;
} CRenderInfo;

typedef struct CRenderInst {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 objType;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x40 - 0x10];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0x48 - 0x44];
    /* 0x048 */ u8 mtx[2][0x40];
    /* 0x0C8 */ u8 bounds[2][0x18];
    /* 0x0F8 */ CRenderInfo *info;
    /* 0x0FC */ u8 padFC[0x108 - 0xFC];
    /* 0x108 */ struct CRenderInst *link;
    /* 0x10C */ CGameObjectInstance *actor;
    /* 0x110 */ u8 pad110[0x1D0 - 0x110];
    /* 0x1D0 */ struct CRenderInst *next;
} CRenderInst;

/* The parent actor block reached through actor->m_AI.m_pParent. */
typedef struct {
    /* 0x000 */ u8 pad0[0x934];
    /* 0x934 */ s16 flags934[1];
} Parent0023181C;

void func_0023181C(CRenderInst *arg0) {
    s32 temp_s0;
    u16 temp_v1;
    u8 temp_a0;
    CRenderInst *temp_s0_2;

    if (arg0->objType == 0x68) {
        temp_s0 = (s32) arg0->actor->m_AI.m_pParent;
        ((Parent0023181C *)((*func_0025398C(temp_s0, 0x15) * 2) + temp_s0))->flags934[0] = 1;
    }
    temp_s0_2 = arg0->link;
    if (temp_s0_2 != NULL) {
        temp_a0 = temp_s0_2->type;
        if ((temp_a0 == 2) && (temp_s0_2->flags & 0x100) && ((temp_v1 = temp_s0_2->objType, (temp_v1 == temp_a0)) || (temp_v1 == 0x111))) {
            func_0022F1C8(&D_800F8928, temp_s0_2);
            func_00232B40(temp_s0_2);
        }
    }
    func_00232B40(arg0);
}
