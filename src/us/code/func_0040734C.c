#include "common.h"
#include "structs/game_object.h"

/* Two flags at D_8012F9E0; the word 8 bytes before it is D_8012F9D8. */
typedef struct {
    /* 0x0 */ s8 unk0;
    /* 0x1 */ s8 unk1;
} BossFlags;

extern BossFlags D_8012F9E0;

void func_0040734C(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
    D_8012F9E0.unk0 = 0;
    D_8012F9E0.unk1 = 0;
    ((s32 *) &D_8012F9E0)[-2] = 1;
}
