/* Second-module twin of Primagen_RunAttackStep (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#define BOSS_STAGE_AS_WORD
#include "structs/boss.h"

extern s32 D_8012F608;           /* stage */
extern f32 Random_Blend(f32 lo, f32 hi);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

/* An attack: AI mode and the distance window it can start from (0x10 bytes). */
typedef struct {
    /* 0x0 */ s32 mode;
    /* 0x4 */ f32 minDist;
    /* 0x8 */ f32 maxDist;
    /* 0xC */ s32 padC;
} BossAttack;

typedef struct {
    /* 0x0 */ f32 waitMin;       /* 0x8012F618 */
    /* 0x4 */ f32 waitMax;       /* 0x8012F61C */
} BossWait;

extern BossWait D_8012F618;
extern BossAttack D_00430E30[];
extern BossStep D_00430F28[];
extern BossStep D_00430FB4[];
extern BossStep D_00431004[];
extern BossStep D_004310B8[];
extern BossStep D_00431158[];
extern BossStep D_00431220[];

s32 Primagen_RunAttackStep_2(CGameObjectInstance *obj, CAI *ai, f32 dist) {
    f32 scale;

    if (D_8012F9DC.script.timer > 0.0f) {
        return 0;
    }
    switch (D_8012F9DC.script.steps[D_8012F9DC.script.current].type) {
        case 2:
            if (D_8012F9DC.script.steps[D_8012F9DC.script.current].arg == 1) {
                D_8012F618.waitMax = 2.5f;
                D_8012F618.waitMin = 0.0f;
            }
            if (D_8012F9DC.script.steps[D_8012F9DC.script.current].arg == 2) {
                D_8012F618.waitMin = Random_Blend(0.0f, 2.0f);
                D_8012F618.waitMax = Random_Blend(3.0f, 4.5f);
            }
            if (D_8012F9DC.script.steps[D_8012F9DC.script.current].arg == 3) {
                D_8012F618.waitMax = 4.0f;
                D_8012F618.waitMin = 1.0f;
            }
            D_8012F9DC.mode = 9;
            CAI__SetMode(obj, ai, 0x12);
            return 1;
        case 3:
            scale = 10.24f;
            if (D_00430E30[D_8012F9DC.script.steps[D_8012F9DC.script.current].arg].minDist * scale <= dist
                && dist <= D_00430E30[D_8012F9DC.script.steps[D_8012F9DC.script.current].arg].maxDist * scale) {
                obj->m_AI.m_AnimSpeed = D_8012F9DC.script.steps[D_8012F9DC.script.current].animSpeed;
                CAI__SetMode(obj, ai, D_00430E30[D_8012F9DC.script.steps[D_8012F9DC.script.current].arg].mode);
                return 1;
            }
            break;
        case 4:
        case 5:
            break;
        case 6:
            if (D_8012F608 == 1) {
                D_8012F9DC.script.steps = D_00430FB4;
            }
            if (D_8012F608 == 2) {
                D_8012F9DC.script.steps = D_004310B8;
            }
            if (D_8012F608 == 3) {
                D_8012F9DC.script.steps = D_00431220;
            }
            D_8012F9DC.script.cursor = -1;
            D_8012F9DC.script.current = -1;
            D_8012F9DC.mode = 4;
            CAI__SetMode(obj, ai, 0x12);
            return 1;
        case 7:
            if (D_8012F608 == 1) {
                D_8012F9DC.script.steps = D_00430F28;
            }
            if (D_8012F608 == 2) {
                D_8012F9DC.script.steps = D_00431004;
            }
            if (D_8012F608 == 3) {
                D_8012F9DC.script.steps = D_00431158;
            }
            D_8012F9DC.script.cursor = -1;
            D_8012F9DC.script.current = -1;
            D_8012F9DC.mode = 2;
            CAI__SetMode(obj, ai, 0x18);
            return 1;
    }
    return 0;
}
