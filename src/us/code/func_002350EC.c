/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A5588.c, func_002A5588, with Turok 2's layouts. */

#include "common.h"

typedef struct TrailActorDef {
    /* 0x00 */ s32 flags;           /* bit 31: ..., 0x10000: keyed trail */
} TrailActorDef;

typedef struct TrailActor {
    /* 0x000 */ u8 pad0[0x2];
    /* 0x002 */ u16 id;
    /* 0x004 */ u8 pad4[0xF8 - 0x4];
    /* 0x0F8 */ TrailActorDef *def;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ s32 key;
    /* 0x110 */ u8 pad110[0x1B5 - 0x110];
    /* 0x1B5 */ u8 trailCount;
} TrailActor;

typedef struct TrailInfo {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s16 sound;
    /* 0x12 */ u8 pad12[0xD];
    /* 0x1F */ u8 tex0;
    /* 0x20 */ u8 tex1;
} TrailInfo;

typedef struct Trail Trail;
struct Trail {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Trail *next;
    /* 0x08 */ TrailInfo *info;
    /* 0x0C */ s32 tex0;
    /* 0x10 */ s32 tex1;
    /* 0x14 */ u8 sound[8];
    /* 0x1C */ s32 owner;
    /* 0x20 */ s32 id;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 soundId;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 flags;
    /* 0x40 */ u8 nodes[0xC];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
};

typedef struct TrailNode {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ TrailActor *owner;
} TrailNode;

typedef struct TrailMgr {
    /* 0x0000 */ u8 pad0[0x7514];
    /* 0x7514 */ u8 freeList[0xC];
    /* 0x7520 */ Trail *trails;
} TrailMgr;

extern s32 D_800ACFB0[];
extern u8 D_800F7078[];

void func_0020F79C(void *m);
void func_0026D230(void *p);
Trail *func_0026D3BC(void *list);
void func_002316A0(void *a, void *b);
TrailInfo *func_00223F50(void *tbl, s32 arg);
void func_00207914(void *p, s32 v);
TrailNode *func_002333C0(void *o, Trail *s, void *m);

/* Adds a node to the actor's trail, starting the trail first when the actor has none. */
void func_002350EC(TrailMgr *mgr, TrailActor *a, s32 type) {
    u8 m[0x40];
    Trail *s;
    register Trail *q asm("$5");
    s32 qid;
    f32 start;
    TrailInfo *t;
    TrailNode *node;
    s32 tex0;
    s32 tex1;
    s32 key;
    s32 id;

    if (a->def->flags & 0x10000) {
        key = a->key;
        id = a->id;
        s = NULL;
        q = mgr->trails;
        while (q != NULL) {
            if (q->owner == key) {
                qid = q->id;
                s = q;
                if (qid == id) {
                    break;
                }
            }
            q = q->next;
            s = NULL;
        }
    } else {
        s = NULL;
    }
    if (s == NULL) {
        s = func_0026D3BC(mgr->freeList);
        if (s == NULL) {
            return;
        }
        t = func_00223F50(D_800F7078, type);
        tex0 = D_800ACFB0[t->tex0];
        tex1 = D_800ACFB0[t->tex1];
        func_0026D230(s->nodes);
        s->flags = 2;
        if (a->def->flags < 0) {
            s->flags = 6;
        }
        if (a->def->flags & 0x10000) {
            s->flags |= 8;
            s->owner = a->key;
            s->id = a->id;
        } else {
            s->owner = (s32)a;
            s->id = -1;
        }
        s->info = t;
        s->tex0 = tex0;
        s->tex1 = tex1;
        start = t->unk0;
        s->unk28 = 0;
        s->unk24 = start;
        s->soundId = t->sound;
        s->unk4C = 0;
        s->unk50 = 0;
        s->unk2C = 0;
        s->unk30 = 0;
        s->unk38 = -1;
        if (s->soundId != -1) {
            func_00207914(s->sound, s->soundId);
            if (a != NULL) {
                func_002316A0(a, s->sound);
            }
        }
        if (s == NULL) {
            return;
        }
    }
    a->trailCount++;
    if (a->def->flags & 0x10000) {
        func_0020F79C(m);
        node = func_002333C0(mgr, s, m);
        if (node != NULL) {
            node->owner = a;
        }
    }
}
