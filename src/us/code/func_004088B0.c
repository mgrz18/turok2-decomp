#include "common.h"
#include "structs/game_object.h"

/* The actor's CAI (at 0x140), with the fields this sets. */
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad04[0xC0];
    /* 0xC4 */ s16 animType;
    /* 0xC6 */ s8 animIndex;
    /* 0xC7 */ s8 animRepeats;
} BossAI;

/* An actor, with the fields this sets (CAI at 0x140). */
typedef struct {
    /* 0x000 */ u8 pad000[0x14];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad018[0x128];
    /* 0x140 */ BossAI ai;
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

static __inline__ void startBossAnim(BossActor *obj) {
    BossActor *boss = D_8012FB50[0];
    BossAI *ai = &boss->ai;

    D_8012FB64[0] = 0;
    ai->animIndex = func_00219F74(boss, 0x5334, -1);
    ai->animType = 0x5334;
    ai->flags &= ~1;
    ai->animRepeats = 0;
    func_0027AD24(D_801100F0, obj, "Enemies", ai);
}

void func_004088B0(BossActor *obj, BossEvents *events) {
    events->onUpdate = func_004085A0;
    events->onEvent = func_00408FF0;
    D_8012FB50[0] = obj;
    D_8012FB54[0] = (u8 *) obj->def + 0x14;
    D_8012FB5C[0] = obj->def->m_Destructible.m_TotemMissionDuration * 15;
    D_8012FB58[0] = 0;
    D_8012FB60[0] = 0;
    obj->onDraw = func_004089E0;
    startBossAnim(obj);
    func_0027AD54(D_801100F0, obj, "Time");
}
