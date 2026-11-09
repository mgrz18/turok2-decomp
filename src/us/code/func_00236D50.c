/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7480.c, func_002A7480.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
#ifdef TARGET_PORT
#include "structs/swoosh.h"
#endif
typedef struct Emit Emit;
typedef struct EmitDef {
               u8 pad0[2];
               u8 unk2;
               u8 unk3;
               u8 pad4[0x19];
               u8 unk1D;
               u8 pad1E[3];
               u8 unk21;
               u8 unk22;
               u8 unk23;
               s32 unk24;
} EmitDef;
typedef struct EmitOwner {
                u8 pad0[0x118];
                struct EmitOwnerInfo *info;
                u8 pad11C[0x1F];
                u8 unk13B;
                u8 pad13C[0x9D];
                u8 unk1D9;
} EmitOwner;
typedef struct EmitOwnerInfo {
               u8 pad0[0x14];
               s32 unk14;
} EmitOwnerInfo;
typedef struct Mtx64 {
    f32 m[16];
} Mtx64;
typedef struct EmitNode EmitNode;
struct EmitNode {
               EmitNode *prev;
               EmitNode *next;
               f32 unk8;
               u8 padC[4];
               f32 v10[3];
               f32 v1C[3];
               Mtx64 mtx[2];
               f32 unkA8;
               f32 unkAC;
               EmitOwner *unkB0;
};
struct Emit {   /* structs/swoosh.h Swoosh */
               Emit *prev;
               Emit *next;
               EmitDef *def;
               void *unkC;
               void *unk10;
               s32 texture[2];
               EmitOwner *owner;
               s32 id;
               f32 unk24;
               f32 unk28;
               f32 unk2C;
               f32 unk30;
               s32 unk34;
               u8 pad38[4];
               u32 flags;
               EmitNode *list;
               EmitNode *tail;
               s32 unk48;
               f32 unk4C;
};
typedef struct EmitMgr {
                 u8 pad0[0x7520];
                 Emit *head;     /* 0x7520 in Turok 2 (0x7528 in Rage Wars) */
} EmitMgr;
void Trail_PlaceNodes(Emit *);
void Trail_ComputeFrames(Emit *, s32);
void Trail_DrawTextured(EmitMgr *, Emit *, s32);
void Trail_DrawUntextured(EmitMgr *, Emit *, s32);
/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline void upd(EmitMgr *m, Emit *e, s32 c) {
    if (e->flags & 8) {
        Trail_PlaceNodes(e);
    }
    if (e->flags & 4) {
        Trail_ComputeFrames(e, c);
    }
    if (e->unk48 >= 2) {
        if (e->unk34 >= 0) {
            Trail_DrawTextured(m, e, c);
        } else {
            Trail_DrawUntextured(m, e, c);
        }
    }
}
void func_00236D50(EmitMgr *m, s32 c) {
    Emit *e;
#ifdef TARGET_PORT
    for (e = ((CSwooshSystem *)m)->m_SwooshPool.used.head; e != ((void *)0); e = e->next) {
#else
    for (e = m->head; e != ((void *)0); e = e->next) {
#endif
        if (e->unk34 < 0) {
            upd(m, e, c);
        }
    }
#ifdef TARGET_PORT
    for (e = ((CSwooshSystem *)m)->m_SwooshPool.used.head; e != ((void *)0); e = e->next) {
#else
    for (e = m->head; e != ((void *)0); e = e->next) {
#endif
        if (e->unk34 > 0) {
            upd(m, e, c);
        }
    }
}
