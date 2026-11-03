/* Second VM module copy of func_0042DA60 (same source, linked 0x0042D980). */
#include "common.h"

typedef struct ActorDef {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 pos[3];
} ActorDef;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x34 - 0x18];
    /* 0x034 */ f32 scale[3];
    /* 0x040 */ u8 pad40[0xD4 - 0x40];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x174 - 0x140];
    /* 0x174 */ s8 unk174;
    /* 0x175 */ u8 pad175[0x1A8 - 0x175];
    /* 0x1A8 */ void *owner;
    /* 0x1AC */ u8 pad1AC[0x234 - 0x1AC];
    /* 0x234 */ void *onUpdate;
    /* 0x238 */ void *onDamage;
    /* 0x23C */ u8 pad23C[4];
    /* 0x240 */ void *onCollide;
    /* 0x244 */ void *onMove;
    /* 0x248 */ void *onDraw;
    /* 0x24C */ u8 pad24C[0x288 - 0x24C];
} Actor;

typedef struct Player {
    /* 0x000 */ Actor actor;
    /* 0x288 */ Actor shadow;
    /* 0x510 */ u8 pad510[0x51C - 0x510];
    /* 0x51C */ s32 unk51C;
    /* 0x520 */ u8 pad520[0x996 - 0x520];
    /* 0x996 */ s16 weapon;
} Player;

typedef struct GameMode {
    u8 multiplayer;
} GameMode;

extern GameMode D_8011AAF1;
extern f32 D_800A6A98; /* 0.5f, in the engine's .rodata */

extern void func_0021E6AC(void *obj, void *rec, s32 mode, s32 index);
extern void func_0021E840();
extern void func_00256738();
extern void func_00247C10();
extern void func_0025689C();
extern void func_00247FB0();
extern void func_00257A40();
extern void func_00243414(void *, void *, s32);
extern void func_00258668(void *, void *);
extern void func_0024CF58(Player *p, Player *q);
extern void func_00284254(s32);

void func_00466980(Player *p, void *rec, s32 mode) {
    f32 half;

    p->actor.owner = p;
    func_0021E6AC(p, rec, mode, 0);
    p->actor.owner = p;
    p->actor.flags |= 0x100000;
    p->actor.flags |= 0x10000;
    p->actor.flags |= 0x1000000;
    p->actor.flags |= 0x2000000;
    if (!D_8011AAF1.multiplayer) {
        p->actor.flags |= 0x10000000;
    }
    p->actor.onMove = func_0021E840;
    p->actor.onDraw = func_00256738;
    p->actor.onDamage = func_00247C10;
    p->actor.onCollide = func_0025689C;
    if (D_8011AAF1.multiplayer) {
        p->actor.onUpdate = func_00247FB0;
    }

    p->shadow.owner = p;
    func_0021E6AC(&p->shadow, rec, mode, 0);
    p->shadow.onMove = func_00257A40;
    p->shadow.unk174 = -1;
    p->shadow.owner = p;
    p->shadow.onDamage = func_00247C10;
    p->shadow.onCollide = func_0025689C;
    p->shadow.flags |= 0x200000;
    p->weapon = -1;
    half = D_800A6A98;
    p->shadow.scale[0] = half;
    p->shadow.scale[1] = half;
    p->shadow.scale[2] = half;
    p->shadow.flags &= ~0x10000;
    func_00243414(&p->shadow, p->shadow.ai, 0);
    func_00258668(&p->shadow, p->shadow.ai);
    func_0024CF58(p, p);
    func_00284254(p->unk51C);
    p->actor.scale[0] = p->actor.def->pos[0];
    p->actor.scale[1] = p->actor.def->pos[1];
    p->actor.scale[2] = p->actor.def->pos[2];
}
