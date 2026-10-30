#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ u8 pad08[0x88 - 0x08];
} Targets;

/* Boss module state at 0x8012F608. */
typedef struct {
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad004[0x3C8];
    /* 0x3CC */ s32 unk3CC;         /* 0x8012F9D4 */
    /* 0x3D0 */ s32 state;          /* 0x8012F9D8 */
    /* 0x3D4 */ s32 mode;           /* 0x8012F9DC */
    /* 0x3D8 */ u8 pad3D8[0x50];
    /* 0x428 */ CVector3 dest;      /* 0x8012FA30 */
} BossModule;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);

extern s32 D_800F1F50[];
extern CVector3 *D_800C18F0[];  /* boss paths */
extern s32 D_8012F600;          /* point on the path */
extern s32 D_8012FB48;          /* path */
extern BossModule D_8012F608;

void func_004074D0(CGameObjectInstance *obj, CAI *ai) {
    Targets t;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 0, 1);
    D_800F1F50[0] &= ~0x8000;
    obj->m_AI.m_AnimSpeed = 1.0f;
    obj->m_dwFlags |= 0x2000;
    if (D_8012F608.mode == 11) {
        D_8012F608.state = 11;
    }
    if (D_8012F608.mode == 4) {
        if (D_8012F608.stage != 1 && D_8012F608.state != 4) {
            D_8012FB48 = 0;
            D_8012F600 = 0;
            D_8012F608.dest = D_800C18F0[0][0];
        }
        D_8012F608.state = 4;
        D_8012F608.unk3CC = 3;
    }
    if (D_8012F608.mode == 3) {
        D_8012F608.state = 3;
        D_8012F608.unk3CC = 3;
    }
}
