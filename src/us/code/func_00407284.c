#include "common.h"
#include "structs/game_object.h"

/* Boss module state at 0x8012F9D8. */
typedef struct {
    /* 0x0 */ s32 state;
    /* 0x4 */ s32 mode;
} BossState;

extern BossState D_8012F9D8;

void func_00407284(CGameObjectInstance *obj) {
    if (D_8012F9D8.mode == 5) {
        D_8012F9D8.state = 5;
    } else if (D_8012F9D8.mode == 6) {
        D_8012F9D8.state = 6;
    } else if (D_8012F9D8.mode == 10) {
        D_8012F9D8.state = 10;
    } else {
        D_8012F9D8.state = 6;
    }
    obj->m_dwFlags |= 0x2000;
}
