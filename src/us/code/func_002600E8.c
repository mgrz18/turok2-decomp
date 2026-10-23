#include "common.h"

typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ u8 pad10[0x4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xD4 - 0x18];
    /* 0x0D4 */ union {
        u32 word;
        struct {
            u32 hi : 12;
            u32 b80000 : 1;
            u32 b : 5;
            u32 b2000 : 1;
            u32 c : 4;
            u32 b100 : 1;
            u32 lo : 8;
        } bits;
    } flags;
} Actor;

extern s32 D_800F7078[];

void func_00246690(Actor *self, s32 type, Vec3 pos, s32 arg4);
void func_004089C8(void);
void func_00220260(void *mgr, Actor *actor, s32 flag);
void func_0026E110(Actor *actor, s32 flags, Actor *actor2);
void func_00246918(Actor *self, void *ai);
void func_002280B0(Actor *self);

void func_002600E8(Actor *self, void *ai) {
    ActorDef *def;

    func_00246690(self, 0x17, self->pos, 0);
    func_004089C8();
    def = self->def;
    if (!(self->flags.word & 0x80000)) {
        if (!(def->flags & 0x20)) {
            func_00220260(D_800F7078, self, 1);
        }
        func_0026E110(self, 1, self);
    }
    self->flags.bits.b100 = 0;
    self->flags.bits.b2000 = 0;
    func_00246918(self, ai);
    if (self->flags.word & 0x80000) {
        func_002280B0(self);
    }
}
