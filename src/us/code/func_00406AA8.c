#include "common.h"
#include "structs/game_object.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

typedef struct BossStep2 BossStep2;

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
    /* 0x400 */ u8 pad400[0x4];
    /* 0x404 */ BossStep2 *script2; /* 0x8012FA0C */
    /* 0x408 */ s32 index2;         /* 0x8012FA10 */
    /* 0x40C */ s32 current2;       /* 0x8012FA14 */
    /* 0x410 */ s8 script2On;       /* 0x8012FA18 */
    /* 0x411 */ u8 pad411[0x17];
    /* 0x428 */ CVector3 dest;      /* 0x8012FA30 */
    /* 0x434 */ CVector3 home;      /* 0x8012FA3C */
} BossModule;

extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern BossModule D_8012F608;
extern BossStep2 D_004313B4[];
extern BossStep2 D_00431424[];
extern BossStep2 D_004314CC[];
extern BossStep2 D_00431558[];
extern BossStep2 D_00431590[];

/* func_004070B4: start the second script for a stage. */
static __inline__ void StartScript2(s32 stage) {
    D_8012F608.index2 = -1;
    D_8012F608.script2On = 1;
    D_8012F608.current2 = -1;
    if (stage == 0) {
        D_8012F608.script2 = NULL;
    }
    if (stage == 1) {
        D_8012F608.script2 = D_004313B4;
    }
    if (stage == 2) {
        D_8012F608.script2 = D_00431424;
    }
    if (stage == 3) {
        D_8012F608.script2 = D_004314CC;
    }
    if (stage == 4) {
        D_8012F608.script2 = D_00431558;
    }
    if (stage == 5) {
        D_8012F608.script2 = D_00431590;
    }
}

void func_00406AA8(CGameObjectInstance *obj, CAI *ai) {
    f32 dy;

    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    dy = obj->header.m_vPos.y - D_8012F608.dest.y;
    if (!(ABS(dy) < 10.0f)) {
        return;
    }
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.y = 0.0f;
    obj->m_vVelocity.z = 0.0f;
    D_8012F608.unk3CA = 0;
    D_8012F608.index = -1;
    D_800F1C40[0] &= ~0x300;
    D_800F1F50[0] &= ~0x300;
    if (D_8012F608.state == 3) {
        obj->header.m_vPos.x = D_8012F608.home.x;
        obj->header.m_vPos.y = D_8012F608.home.y;
        obj->header.m_vPos.z = D_8012F608.home.z;
        obj->m_RotY = 0.0f;
        D_8012F608.mode = 1;
        if (D_8012F608.stage == 1) {
            StartScript2(1);
        }
        if (D_8012F608.stage == 2) {
            StartScript2(2);
        }
        if (D_8012F608.stage == 3) {
            StartScript2(3);
        }
        func_00243414(obj, ai, 5);
    } else if (!D_8012F608.partActive) {
        D_8012F608.mode = 2;
        func_00243414(obj, ai, 7);
    } else {
        func_00243414(obj, ai, 0x10);
    }
}
