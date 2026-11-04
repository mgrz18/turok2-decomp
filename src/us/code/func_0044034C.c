#include "common.h"
#include "structs/game_object.h"

/* D_8012F9E0: two flags; D_8012F9D8 (8 bytes before it) is set through it too. */
typedef struct {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ s8 unk1;
} Flags9E0;

extern Flags9E0 D_8012F9E0;

void func_0044034C(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags |= 0x2000;
    D_8012F9E0.unk0 = 0;
    D_8012F9E0.unk1 = 0;
    ((s32 *) &D_8012F9E0)[-2] = 1;
}
