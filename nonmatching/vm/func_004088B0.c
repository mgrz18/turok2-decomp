/* NON-MATCHING: 63 of 66 words differ: the ROM rereads obj->def after storing D_8012FB50/54 and keeps the boss in s0 without an extra register; GCC caches D_8012FB50's address. */
#include "common.h"
#include "structs/game_object.h"

/* The definition an actor points to at +0x14 (only +0x54 is read). */
typedef struct {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ s32 seconds;
} BossDef;

/* An actor, with the fields this sets (CAI at 0x140). */
typedef struct {
    /* 0x000 */ u8 pad000[0x14];
    /* 0x014 */ BossDef *def;
    /* 0x018 */ u8 pad018[0x128];
    /* 0x140 */ u32 aiFlags;
    /* 0x144 */ u8 pad144[0xC0];
    /* 0x204 */ s16 animType;       /* ai + 0xC4 */
    /* 0x206 */ s8 animIndex;       /* ai + 0xC6 */
    /* 0x207 */ s8 animRepeats;     /* ai + 0xC7 */
    /* 0x208 */ u8 pad208[0x2C];
    /* 0x234 */ void *onDraw;
} BossActor;

typedef struct {
    /* 0x00 */ u8 pad00[0xF0];
    /* 0xF0 */ void *onUpdate;
    /* 0xF4 */ u8 padF4[0x4];
    /* 0xF8 */ void *onEvent;
} BossEvents;

extern s8 func_00219F74(BossActor *actor, s32 anim, s32 arg2);
extern void func_0027AD24(void *hud, BossActor *actor, char *name, void *ai);
extern void func_0027AD54(void *hud, BossActor *actor, char *name);
extern void func_004085A0();
extern void func_00408FF0();
extern void func_004089E0();

extern u8 D_801100F0[];
/* Boss fight state (0x8012FB50 on). */
extern BossActor *D_8012FB50[];   /* the boss */
extern void *D_8012FB54[];        /* its definition + 0x14 */
extern s32 D_8012FB58[];
extern f32 D_8012FB5C[];          /* time left */
extern s32 D_8012FB60[];
extern s32 D_8012FB64[];

void func_004088B0(BossActor *obj, BossEvents *events) {
    BossActor *boss;

    events->onUpdate = func_004085A0;
    events->onEvent = func_00408FF0;
    D_8012FB50[0] = obj;
    D_8012FB54[0] = (u8 *) obj->def + 0x14;
    D_8012FB58[0] = 0;
    D_8012FB60[0] = 0;
    D_8012FB5C[0] = obj->def->seconds * 15;
    obj->onDraw = func_004089E0;
    boss = D_8012FB50[0];
    D_8012FB64[0] = 0;
    boss->animIndex = func_00219F74(boss, 0x5334, -1);
    boss->animType = 0x5334;
    boss->aiFlags &= ~1;
    boss->animRepeats = 0;
    func_0027AD24(D_801100F0, obj, "Enemies", &boss->aiFlags);
    func_0027AD54(D_801100F0, obj, "Time");
}
