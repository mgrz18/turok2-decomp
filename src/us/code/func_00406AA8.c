#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern BossStep2 D_004313B4[];
extern BossStep2 D_00431424[];
extern BossStep2 D_004314CC[];
extern BossStep2 D_00431558[];
extern BossStep2 D_00431590[];

/* func_004070B4: start the second script for a stage. */
static __inline__ void StartScript2(s32 stage) {
    D_8012F608.script2.cursor = -1;
    D_8012F608.script2On = 1;
    D_8012F608.script2.current = -1;
    if (stage == 0) {
        D_8012F608.script2.steps = NULL;
    }
    if (stage == 1) {
        D_8012F608.script2.steps = D_004313B4;
    }
    if (stage == 2) {
        D_8012F608.script2.steps = D_00431424;
    }
    if (stage == 3) {
        D_8012F608.script2.steps = D_004314CC;
    }
    if (stage == 4) {
        D_8012F608.script2.steps = D_00431558;
    }
    if (stage == 5) {
        D_8012F608.script2.steps = D_00431590;
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
    D_8012F608.script.current = -1;
    D_800F1C40[0] &= ~0x300;
    D_800F1F50[0] &= ~0x300;
    if (D_8012F608.bossState == 3) {
        obj->header.m_vPos.x = D_8012F608.home.x;
        obj->header.m_vPos.y = D_8012F608.home.y;
        obj->header.m_vPos.z = D_8012F608.home.z;
        obj->m_RotY = 0.0f;
        D_8012F608.bossMode = 1;
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
        D_8012F608.bossMode = 2;
        func_00243414(obj, ai, 7);
    } else {
        func_00243414(obj, ai, 0x10);
    }
}
