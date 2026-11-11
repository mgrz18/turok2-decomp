#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "protos/ai.h"

/* Boss fight setup: finds the six level actors with id 0x1DB1 and the one
 * with 0x1DB2, installs the AI callbacks and resets the 15 attack slots. */

#ifdef TARGET_PORT
typedef struct Obj {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Obj(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_Obj(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
    };
} Obj;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
} Obj;
#endif

extern u32 D_800F1F10[];

void ScriptAnimator_Start(void *, Obj *);
void MotherBoss_Step();
void MotherBoss_Draw();
void MotherBoss_Collision();
void EnemyAI_HeadNodeCallback();
extern u8 func_00408FF8[];
#ifdef TARGET_PORT
void MotherBoss_Hurt(void *obj, CAI *ai, void *hit); /* (.., DamageEvent *) */
#endif

void MotherBoss_Init(Obj *obj, CAI *ai) {
    MotherBossState *s = &gMotherBoss;
    s32 i;

    gScene_m_LevelID = 0x1DB1;
    s->pickupTimer = 0;
    s->dropTimer = 0;
    s->partCount = CScene__FindInstances(&gScene, 0x650, 10, 0x1DB1, (CGameObjectInstance **)s->parts, 6);
    for (i = 0; i < s->partCount; i++) {
        ((CGameObjectInstance *)s->parts[i])->m_AI.m_Mother.m_bActive = 1;
    }
    s->partsLeft = s->partCount;
    CScene__FindInstances(&gScene, 0x650, 10, 0x1DB2, (CGameObjectInstance **)&s->controller, 1);
    ((CGameObjectInstance *)s->controller)->m_AI.m_Mother.m_bActive = 1;
    s->intel = (void *)CScene__FindIntelligence(&gScene, 1, 0x1DB1);
    ai->m_pModeTable = sMotherBossModeTable;
    ai->m_pfStep = MotherBoss_Step;
#ifdef TARGET_PORT
    /* func_00408FF8 + 8 is MotherBoss_Hurt in the ROM (splat started its object two junk words
     * early); on the host func_00408FF8 is not code at that distance: the first hit called data */
    ai->m_pfHurt = MotherBoss_Hurt;
#else
    ai->m_pfHurt = func_00408FF8 + 8;
#endif
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    ai->m_pfCollision = MotherBoss_Collision;
    ai->m_pfDraw = MotherBoss_Draw;
    CAI__SetMode(obj, ai, 0);
    gMotherBoss.obj = obj;
    D_800F1F10[0] &= ~0x300;
    obj->flags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        ScriptAnimator_Start(&gMotherBoss.slots[i].anim, obj);
    }
}
