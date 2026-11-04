#include "common.h"
#include "structs/intelligence.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0x174 - 0x18];
    /* 0x174 */ f32 speed;
} Mover;

extern f32 D_800B6D28;

void func_00215868(Mover *arg0) {
    arg0->speed = (f32) (D_800B6D28 * arg0->def->m_Morpher.m_AnimSpeed);
}
