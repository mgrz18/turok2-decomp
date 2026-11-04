#include "common.h"
#include "structs/game_object.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x64 - 0x44];
    /* 0x64 */ f32 timer;
    /* 0x68 */ u8 pad68[0xBC - 0x68];
    /* 0xBC */ f32 unkBC;
} AI;

f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00243414(CGameObjectInstance *self, AI *ai, s32 mode);

void func_00261F18(CGameObjectInstance *self, AI *ai) {
    ai->unkBC = func_00211774(ai->unkBC, 0.25f, 0.2f);
    self->m_vVelocity.y = func_00211774(self->m_vVelocity.y, 0.0f, 1.024f);
    if (ai->unk40 >= ai->timer) {
        self->m_dwFlags |= 0x20000;
        func_00243414(self, ai, 6);
    }
}
