#include "common.h"
#include "structs/game_object.h"

extern f32 func_002119FC(f32 lo, f32 hi);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

/* One step of a boss attack script (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ f32 animSpeed;
    /* 0x0C */ u8 pad0C[0x8];
} BossStep;

/* An attack: AI mode and the distance window it can start from (0x10 bytes). */
typedef struct {
    /* 0x0 */ s32 mode;
    /* 0x4 */ f32 minDist;
    /* 0x8 */ f32 maxDist;
    /* 0xC */ s32 padC;
} BossAttack;

/* Boss module state (0x8012F608 on): separate globals. */
typedef struct {
    /* 0x0 */ s32 mode;          /* 0x8012F9DC */
    /* 0x04 */ u8 pad4[0x18];
    /* 0x1C */ f32 cooldown;     /* 0x8012F9F8 */
    /* 0x20 */ BossStep *script; /* 0x8012F9FC */
    /* 0x24 */ s32 scriptPrev;   /* 0x8012FA00 */
    /* 0x28 */ s32 scriptIndex;  /* 0x8012FA04 */
} BossMode;
typedef struct {
    /* 0x0 */ f32 waitMin;       /* 0x8012F618 */
    /* 0x4 */ f32 waitMax;       /* 0x8012F61C */
} BossWait;

extern s32 D_8012F608;           /* stage */
extern BossWait D_8012F618;
extern BossMode D_8012F9DC;
extern BossAttack D_00430F10[];
extern BossStep D_00431008[];
extern BossStep D_00431094[];
extern BossStep D_004310E4[];
extern BossStep D_00431198[];
extern BossStep D_00431238[];
extern BossStep D_00431300[];

s32 func_00404C44(CGameObjectInstance *obj, CAI *ai, f32 dist) {
    f32 scale;

    if (D_8012F9DC.cooldown > 0.0f) {
        return 0;
    }
    switch (D_8012F9DC.script[D_8012F9DC.scriptIndex].type) {
        case 2:
            if (D_8012F9DC.script[D_8012F9DC.scriptIndex].arg == 1) {
                D_8012F618.waitMax = 2.5f;
                D_8012F618.waitMin = 0.0f;
            }
            if (D_8012F9DC.script[D_8012F9DC.scriptIndex].arg == 2) {
                D_8012F618.waitMin = func_002119FC(0.0f, 2.0f);
                D_8012F618.waitMax = func_002119FC(3.0f, 4.5f);
            }
            if (D_8012F9DC.script[D_8012F9DC.scriptIndex].arg == 3) {
                D_8012F618.waitMax = 4.0f;
                D_8012F618.waitMin = 1.0f;
            }
            D_8012F9DC.mode = 9;
            func_00243414(obj, ai, 0x12);
            return 1;
        case 3:
            scale = 10.24f;
            if (D_00430F10[D_8012F9DC.script[D_8012F9DC.scriptIndex].arg].minDist * scale <= dist
                && dist <= D_00430F10[D_8012F9DC.script[D_8012F9DC.scriptIndex].arg].maxDist * scale) {
                obj->m_AI.m_AnimSpeed = D_8012F9DC.script[D_8012F9DC.scriptIndex].animSpeed;
                func_00243414(obj, ai, D_00430F10[D_8012F9DC.script[D_8012F9DC.scriptIndex].arg].mode);
                return 1;
            }
            break;
        case 4:
        case 5:
            break;
        case 6:
            if (D_8012F608 == 1) {
                D_8012F9DC.script = D_00431094;
            }
            if (D_8012F608 == 2) {
                D_8012F9DC.script = D_00431198;
            }
            if (D_8012F608 == 3) {
                D_8012F9DC.script = D_00431300;
            }
            D_8012F9DC.scriptPrev = -1;
            D_8012F9DC.scriptIndex = -1;
            D_8012F9DC.mode = 4;
            func_00243414(obj, ai, 0x12);
            return 1;
        case 7:
            if (D_8012F608 == 1) {
                D_8012F9DC.script = D_00431008;
            }
            if (D_8012F608 == 2) {
                D_8012F9DC.script = D_004310E4;
            }
            if (D_8012F608 == 3) {
                D_8012F9DC.script = D_00431238;
            }
            D_8012F9DC.scriptPrev = -1;
            D_8012F9DC.scriptIndex = -1;
            D_8012F9DC.mode = 2;
            func_00243414(obj, ai, 0x18);
            return 1;
    }
    return 0;
}
