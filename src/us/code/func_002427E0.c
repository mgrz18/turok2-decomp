#include "common.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/intelligence.h"
#include "globals/game.h"

#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): the view at the host offsets of CAI
 * (tools/port/hostview.py). By N64 offset, the aggression (0xC8) landed inside the host CAI's
 * earlier members (pointers widen the CAI before it) and every enemy kept m_Aggression 0: its
 * projectile attack delay was 60 instead of 15 (EnemyMode_Attack_Enter), so demo 1's enemies
 * shot less often than on the N64, and the aggression-scaled dodges and stuns differed. */
#include "structs/game_object.h"
typedef struct Dest {
#define T2_HOFF_Dest(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_health[T2_HOFF_Dest(m_Health)]; s32 health; }; /* N64 0x4 */
        struct { u8 _hp_maxHealth[T2_HOFF_Dest(m_InitialHealth)]; s32 maxHealth; }; /* N64 0x8 */
        struct { u8 _hp_handle[T2_HOFF_Dest(m_VisBits)]; s32 handle; }; /* N64 0xC */
        struct { u8 _hp_unk10[T2_HOFF_Dest(m_TranqHealth)]; s8 unk10; }; /* N64 0x10 */
        struct { u8 _hp_level[T2_HOFF_Dest(m_nTexture)]; s8 level; }; /* N64 0x11 */
        struct { u8 _hp_unkC8[T2_HOFF_Dest(m_Aggression)]; u8 unkC8; }; /* N64 0xC8 */
    };
} Dest;
#else
typedef struct Dest {
    char pad0[4];
    s32 health;        /* 0x04 */
    s32 maxHealth;     /* 0x08 */
    s32 handle;        /* 0x0C */
    s8 unk10;          /* 0x10 */
    s8 level;          /* 0x11 */
    char pad12[0xC8 - 0x12];
    u8 unkC8;          /* 0xC8 */
} Dest;
#endif


/* Initial health and level of an actor from its definition, scaled by difficulty. */
void AI_InitHealth(Dest *dst, CIntelligence *def, CVector3 pos, void *actor) {
    GameSettings *settings;
    s32 level;
    f32 health;
    f32 a;
    f32 b;
    f32 t;

    level = 0;
    a = 100.0f;
    b = 128.0f;
    dst->handle = -1;
    health = a;
    if (def != NULL) {
        settings = &gEngineApp_m_Settings;
        level = def->m_nTexture;
        if (settings->multiplayer != 0 && def->m_Type != 1 && def->m_Type != 11) {
            if (settings->match.tag != 0) {
                level = settings->tagTextureSet;
            } else {
                level = settings->dmTextureSet;
            }
        }
        switch (def->m_Type) {
            case 1:
                health = def->m_Enemy.m_Health;
                a = def->m_Enemy.m_TranqHealth;
                b = def->m_Enemy.m_Aggression;
                break;
            case 4:
                health = def->m_Enemy.m_Health;
                break;
            case 7:
                health = def->m_Destructible.m_Health;
                break;
        }
        if (def->m_Type == 1 || def->m_Type == 4) {
            switch (gEngineApp_m_Settings.difficulty) {
                case 8:
                    t = health * 0.5f;
                    b = 0.0f;
                    if (t < 1.0f) {
                        t = 1.0f;
                    }
                    health = t;
                    break;
                case 32:
                    b = 255.0f;
                    health *= 1.25f;
                    break;
            }
        }
    }
    dst->health = health;
    dst->unk10 = a;
    dst->level = level;
    dst->unkC8 = (s32)b;
    dst->health <<= 8;
    dst->maxHealth = dst->health;
}
