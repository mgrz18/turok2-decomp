#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/collision.h"
#include "globals/collision.h"
#include "globals/ai.h"
#include "protos/ai.h"

/* AI hit callback: a projectile (or its owner) close enough lengthens the
 * wait and becomes the last-heard position; then the projectile type picks a
 * reaction animation (flinch, burn, freeze, shock...) the definition allows. */

typedef struct Projectile {
    /* 0x000 */ u8 pad0[0x2];
    /* 0x002 */ u16 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x10C - 0x10];
    /* 0x10C */ struct Projectile *owner;
    /* 0x110 */ u8 pad110[0x1BC - 0x110];
    /* 0x1BC */ f32 charge;
} Projectile;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern f32 CVector3__Distance(CVector3 *a, CVector3 *b);
extern void Sound__Play(s16 sound, CVector3 pos, sptr, sptr);

/* A noise at pos within 3072 units lengthens the wait and is remembered. */
static __inline__ void hearNoise(CGameObjectInstance *actor, CAI *ai, CVector3 *pos) {
    s32 mode;
    f32 dist;
    f32 add;

    mode = ai->m_Enemy.m_Behavior[ai->m_Enemy.m_BehaviorIndex];
    if (mode >= 0) {
        if (mode < 3) {
            dist = CVector3__Distance(&actor->header.m_vPos, pos);
            if (dist < 3072.0f) {
                add = (3072.0f - dist) * 0.048828125f;
                if (ai->m_ActionTime < add) {
                    ai->m_ActionTime += add;
                    if (add < ai->m_ActionTime) {
                        ai->m_ActionTime = add;
                    }
                    gEnemyLastNoisePos = *pos;
                }
            }
        }
    }
}

/* level and delay are function-wide variables shared by the three reactions
 * (their register choices depend on it). */
void EnemyAI_OnHit(CGameObjectInstance *actor, CAI *ai, Projectile *proj) {
    CEnemyIntelligence *info;
    Projectile *owner;
    s32 v;
    f32 t;
    f32 level;
    f32 delay;

    info = &actor->header.m_pIntelligence->m_Enemy;
    if (ai->m_Health == 0) {
        return;
    }
    owner = proj->owner;
    if (owner != NULL) {
        hearNoise(actor, ai, &owner->pos);
    } else {
        hearNoise(actor, ai, &proj->pos);
    }
    switch (proj->type) {
    case 3:
        if ((info->m_dwFlags & 0x200) && ai->m_Mode != 0x3D) {
            if (--ai->m_TranqHealth == 0) {
                ai->m_TranqHealth += info->m_TranqHealth;
                if (ai->m_Mode == 0x1D) {
                    v = ai->m_Aggression;
                    v += 10;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    ai->m_Aggression = v;
                    level = ai->m_Aggression;
                    level *= (1.0f / 255);
                    delay = level * -120.0f + 240.0f;
                    ai->m_ConditionTimer += delay;
                } else {
                    CAI__SetMode(actor, ai, 0x1D);
                }
            }
        }
        break;
    case 0x128:
        proj->charge = 0.0f;
    case 4:
    case 0x1CC:
    case 0x1CD:
    case 0x1CE:
    case 0x1CF:
    case 0x1D0:
    case 0x1D1:
    case 0x1D2:
        if ((info->m_dwFlags & 0x100) && ai->m_Mode != 0x3D) {
            if (ai->m_Mode == 0x20) {
                v = ai->m_Aggression;
                v += 10;
                if (v >= 0x100) {
                    v = 0xFF;
                }
                ai->m_Aggression = v;
                ai->m_ConditionTimer = MIN(ai->m_ConditionTimer += 15.0f, 300.0f);
            } else {
                CAI__SetMode(actor, ai, 0x20);
                ai->m_ConditionTimer = proj->charge * 300.0f + 15.0f;
            }
        }
        break;
    case 10:
        if (info->m_dwFlags & 0x80) {
            if (ai->m_Mode == 0x1F) {
                v = ai->m_Aggression;
                v += 10;
                if (v >= 0x100) {
                    v = 0xFF;
                }
                ai->m_Aggression = v;
                level = ai->m_Aggression;
                level *= (1.0f / 255);
                delay = level * -45.0f + 105.0f;
                ai->m_ConditionTimer += delay;
            } else {
                CAI__SetMode(actor, ai, 0x1F);
            }
        }
        break;
    case 11:
    case 0x129:
        if (gCollisionResults.m_pInstance != 0 && (gCollisionResults.m_Frame.flags & 3) == 1) {
            if (info->m_dwFlags & 0x400) {
                Sound__Play(0x1AB, actor->header.m_vPos, 0, -1);
                if (ai->m_Mode == 0x21) {
                    v = ai->m_Aggression;
                    v += 20;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    ai->m_Aggression = v;
                    level = ai->m_Aggression;
                    level *= (1.0f / 255);
                    delay = level * -15.0f + 75.0f;
                    ai->m_ConditionTimer += delay;
                } else {
                    ai->m_vBoreCollNormal = gCollisionResults.m_vNormal;
                    CAI__SetMode(actor, ai, 0x21);
                }
            } else if (info->m_dwFlags & 0x2000) {
                Sound__Play(0x213, actor->header.m_vPos, 0, -1);
                CAI__SetMode(actor, ai, 0x22);
            }
        }
        break;
    case 0x14:
        t = ai->m_OnFireTimer;
        t += 30.0f;
        if (!(t <= 120.0f)) {
            t = 120.0f;
        }
        ai->m_OnFireTimer = t;
        break;
    case 0:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x15:
    case 0x17:
        break;
    }
}
