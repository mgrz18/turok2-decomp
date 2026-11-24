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

sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
void COnScreen__SetBoss(void *, void *, char *);
/* the ROM passes the CAI in a3 too (the definition takes three) */
#define COnScreen__SetBoss_4 ((void (*)(void *, void *, char *, void *)) COnScreen__SetBoss)
void COnScreen__SetBoss2(void *, void *, char *);
extern void TotemDefense_Update();
extern void TotemDefense_OnEvent();
extern void TotemDefense_DrawTotem();

/* Boss fight state (0x8012FB50 on). */
#ifdef TARGET_PORT
#include "structs/boss.h"
#include "globals/boss.h"
#else
/* one-element array views of gTotemDefense and its field symbols (globals/boss.h): through them the ROM's
 * code; asm labels, so the header's names keep their types */
extern BossActor *gTotemDefense_obj_v[] __asm__("gTotemDefense");
extern void *gTotemDefense_def_v[] __asm__("gTotemDefense_def");
extern s32 gTotemDefense_state_v[] __asm__("gTotemDefense_state");
extern f32 gTotemDefense_timer_v[] __asm__("gTotemDefense_timer");
extern s32 gTotemDefense_kills_v[] __asm__("gTotemDefense_kills");
extern s32 gTotemDefense_warned_v[] __asm__("gTotemDefense_warned");
#endif

#ifndef TARGET_PORT
static __inline__ void startBossAnim(BossActor *obj) {
    BossActor *boss = gTotemDefense_obj_v[0];
    BossAI *ai = &boss->ai;

    gTotemDefense_warned_v[0] = 0;
    ai->animIndex = CGameObjectInstance__PickRandomAnim(boss, 0x5334, -1);
    ai->animType = 0x5334;
    ai->flags &= ~1;
    ai->animRepeats = 0;
    COnScreen__SetBoss_4(&gEngineApp_m_OnScreen, obj, "Enemies", ai);
}

#endif
#ifdef TARGET_PORT
/* The port (round 9 x1): Destructible_Setup's totem (type 0x64D) by member: the AI callbacks
 * (N64 0xF0/0xF8 of the CAI: m_pfStep, m_pfHurt), the definition's body (intelligence + 0x14 =
 * m_Destructible), the draw callback (N64 0x234 = m_AI.m_pfDraw), the totem's animation on its
 * CAI and gTotemDefense by member. Through the N64 views the host wrote the callbacks and the
 * animation into other CAI fields: the Harbor Totem's defense never ran its own update. */
DEFERRED void TotemDefense_Init(BossActor *obj_, BossEvents *events_) {
    CGameObjectInstance *obj = (CGameObjectInstance *) obj_;
    CAI *events = (CAI *) events_;
    CAI *ai;

    events->m_pfStep = (void *) TotemDefense_Update;
    events->m_pfHurt = (void *) TotemDefense_OnEvent;
    gTotemDefense.obj = obj;
    gTotemDefense.def = &obj->header.m_pIntelligence->m_Destructible;
    gTotemDefense.timer = (f32) (obj->header.m_pIntelligence->m_Destructible.m_TotemMissionDuration * 15);
    gTotemDefense.state = 0;
    gTotemDefense.kills = 0;
    obj->m_AI.m_pfDraw = (void *) TotemDefense_DrawTotem;
    gTotemDefense.warned = 0;
    ai = &gTotemDefense.obj->m_AI;
    ai->m_AnimIndex = CGameObjectInstance__PickRandomAnim((BossActor *) gTotemDefense.obj, 0x5334, -1);
    ai->m_AnimType = 0x5334;
    ai->m_dwFlags &= ~1;
    ai->m_AnimRepeatedCount = 0;
    COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj_, "Enemies");
    COnScreen__SetBoss2(&gEngineApp_m_OnScreen, obj_, "Time");
}
#else
DEFERRED void TotemDefense_Init(BossActor *obj, BossEvents *events) {
    events->onUpdate = TotemDefense_Update;
    events->onEvent = TotemDefense_OnEvent;
    gTotemDefense_obj_v[0] = obj;
    gTotemDefense_def_v[0] = (u8 *) obj->def + 0x14;
    gTotemDefense_timer_v[0] = obj->def->m_Destructible.m_TotemMissionDuration * 15;
    gTotemDefense_state_v[0] = 0;
    gTotemDefense_kills_v[0] = 0;
    obj->onDraw = TotemDefense_DrawTotem;
    startBossAnim(obj);
    COnScreen__SetBoss2(&gEngineApp_m_OnScreen, obj, "Time");
}
#endif
