#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
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

void func_00246918(Actor *self, void *ai);
void func_002280B0(Actor *self);

void func_00260554(Actor *self, void *ai) {
    self->flags.bits.b100 = 0;
    self->flags.bits.b2000 = 0;
    func_00246918(self, ai);
    if (self->flags.word & 0x80000) {
        func_002280B0(self);
    }
}
