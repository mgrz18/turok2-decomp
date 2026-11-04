#include "common.h"
#include "structs/game_object.h"

s32 func_002026EC(s32, s32);
s32 func_002062F8(s32, s32);
s32 func_002065F8(s32, s32);

extern s32 D_800D1CC0;

void func_002061B8(s32 arg0, CAI *ai) {
    s32 temp_v0;

    ai->m_VisBits = (s32) (ai->m_VisBits & ~2);
    if (ai->m_InitialHealth != 0) {
loop_1:
        do {
            temp_v0 = ai->m_InitialHealth - 1;
            ai->m_InitialHealth = temp_v0;
            if (temp_v0 != 0) {
                goto loop_1;
            }
            ai->m_VisBits = (s32) (ai->m_VisBits & ~0x100);
        } while (ai->m_InitialHealth != 0);
    }
    if (!(ai->m_VisBits & 0x702)) {
        func_002065F8(0, (s32) ai);
        func_002026EC((s32) &D_800D1CC0, ai->m_dwFlags);
        func_002062F8(0, (s32) ai);
    }
}
