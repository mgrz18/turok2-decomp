#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/intelligence.h"
#include "globals/game.h"

#include "structs/game_object.h"


/* Initial health and level of an actor from its definition, scaled by difficulty. Round 28: on CAI
 * (the N64 view and the port's host-offset one of round 10 y3 joined; by N64 offset the host's
 * aggression landed inside earlier members and every enemy kept m_Aggression 0). */
void AI_InitHealth(CAI *dst, CIntelligence *def, CVector3 pos, CGameObjectInstance *actor) {
    GameSettings *settings;
    s32 level;
    f32 health;
    f32 a;
    f32 b;
    f32 t;

    level = 0;
    a = 100.0f;
    b = 128.0f;
    dst->m_VisBits = -1;
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
    dst->m_Health = health;
    dst->m_TranqHealth = (s8) a; /* converted as a signed byte */
    dst->m_nTexture = level;
    dst->m_Aggression = (s32)b;
    dst->m_Health <<= 8;
    dst->m_InitialHealth = dst->m_Health;
}
