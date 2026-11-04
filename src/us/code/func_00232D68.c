#include "common.h"
#include "structs/game_object.h"

extern s32 CList__AddHead(s32, s32);
extern s32 CList__AddTail(s32, s32);
extern s32 func_002005D0(s32, s32, s32);

/* Insert arg1 into the list at 0xEC14: after the node with the same key at
 * 0xF8 if there is one, else at the end or the front by a flag. */
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

typedef struct {
    /* 0x0000 */ u8 pad0[0xEC14];
    /* 0xEC14 */ CRenderInst *drawHead;   /* CList */
} CDrawWorld;

void func_00232D68(CDrawWorld *arg0, CRenderInst *arg1) {
    CRenderInst *p;

    for (p = arg0->drawHead; p != NULL; p = p->next) {
        if (p->info == arg1->info) {
            break;
        }
    }
    if (p != NULL) {
        func_002005D0((s32)&arg0->drawHead, (s32)p, (s32)arg1);
    } else if (arg1->info->flags & 0x2000) {
        CList__AddHead((s32)&arg0->drawHead, (s32)arg1);
    } else {
        CList__AddTail((s32)&arg0->drawHead, (s32)arg1);
    }
    arg1->flags |= 0x01000000;
}
