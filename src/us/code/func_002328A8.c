#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

s32 func_0020B9F4(s32, void *, void *, s32, s32); /* extern */
s32 CScene__GetLightsAt(s32 *, s32, s32);         /* extern */
extern s32 gEvenOdd;

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

void CParticle__DrawModel(CRenderInst *arg0) {
    CScene__GetLightsAt(&gScene, &arg0->pos, arg0->bounds[gEvenOdd]);
    func_0020B9F4(arg0->info->unk14, arg0->mtx[gEvenOdd], arg0->bounds[gEvenOdd], 0, -1);
}
