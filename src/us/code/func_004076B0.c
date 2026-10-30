#include "common.h"
#include "structs/game_object.h"

/* Boss module state at 0x8012F608. */
typedef struct {
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad004[0x3C5];
    /* 0x3C9 */ u8 partActive;      /* 0x8012F9D1 */
    /* 0x3CA */ u8 unk3CA;          /* 0x8012F9D2 */
    /* 0x3CB */ u8 pad3CB[0x5];
    /* 0x3D0 */ s32 state;          /* 0x8012F9D8 */
    /* 0x3D4 */ s32 mode;           /* 0x8012F9DC */
    /* 0x3D8 */ u8 pad3D8[0x24];
    /* 0x3FC */ s32 index;          /* 0x8012FA04 */
} BossModule;

extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_00298470(f32 x);                                          /* sqrt */

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern BossModule D_8012F608;

void func_004076B0(CGameObjectInstance *obj, CAI *ai) {
    CVector3 flat;

    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    if (!(obj->header.m_vPos.y < 10.0f)) {
        return;
    }
    D_8012F608.index = -1;
    D_8012F608.unk3CA = 0;
    D_8012F608.mode = 2;
    flat.x = obj->header.m_vPos.x;
    flat.y = 0.0f;
    flat.z = obj->header.m_vPos.z;
    if (func_00298470(flat.x * flat.x + flat.z * flat.z) < 1638.3999f) {
        D_800F1C40[0] &= ~0x300;
        D_800F1F50[0] &= ~0x300;
    }
    if (!D_8012F608.partActive) {
        func_00243414(obj, ai, 7);
    } else {
        func_00243414(obj, ai, 0x10);
    }
}
