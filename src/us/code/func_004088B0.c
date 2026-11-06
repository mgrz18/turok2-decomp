#include "common.h"
#include "structs/game_object.h"
#include "globals/onscreen.h"

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

extern s8 CGameObjectInstance__PickRandomAnim(BossActor *actor, s32 anim, s32 arg2);
extern void COnScreen__SetBoss(void *hud, BossActor *actor, char *name, void *ai);
extern void COnScreen__SetBoss2(void *hud, BossActor *actor, char *name);
extern void TotemDefense_Update();
extern void TotemDefense_OnEvent();
extern void TotemDefense_DrawTotem();

/* Boss fight state (0x8012FB50 on). */
extern BossActor *gTotemDefense[];   /* the boss */
extern void *gTotemDefense_def[];        /* its definition + 0x14 */
extern s32 gTotemDefense_state[];
extern f32 gTotemDefense_timer[];          /* time left */
extern s32 gTotemDefense_kills[];
extern s32 D_8012FB64[];

static __inline__ void startBossAnim(BossActor *obj) {
    BossActor *boss = gTotemDefense[0];
    BossAI *ai = &boss->ai;

    D_8012FB64[0] = 0;
    ai->animIndex = CGameObjectInstance__PickRandomAnim(boss, 0x5334, -1);
    ai->animType = 0x5334;
    ai->flags &= ~1;
    ai->animRepeats = 0;
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj, "Enemies", ai);
}

void TotemDefense_Init(BossActor *obj, BossEvents *events) {
    events->onUpdate = TotemDefense_Update;
    events->onEvent = TotemDefense_OnEvent;
    gTotemDefense[0] = obj;
    gTotemDefense_def[0] = (u8 *) obj->def + 0x14;
    gTotemDefense_timer[0] = obj->def->m_Destructible.m_TotemMissionDuration * 15;
    gTotemDefense_state[0] = 0;
    gTotemDefense_kills[0] = 0;
    obj->onDraw = TotemDefense_DrawTotem;
    startBossAnim(obj);
    COnScreen__SetBoss2(&gEngineApp_m_OnScreen, obj, "Time");
}
