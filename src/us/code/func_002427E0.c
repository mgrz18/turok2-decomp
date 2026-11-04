#include "common.h"
#include "structs/intelligence.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* Game settings (D_8011AAD8); the difficulty at +8 is read as D_8011AAE0. */
typedef struct Settings {
    char pad0[0x19];
    u8 multiplayer;    /* 0x19 */
    char pad1A[0x23 - 0x1A];
    s8 levelA;         /* 0x23 */
    char pad24[0x28 - 0x24];
    s8 levelB;         /* 0x28 */
    char pad29[0x20C - 0x29];
    s32 useLevelB;     /* 0x20C */
} Settings;

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

extern Settings D_8011AAD8;
extern s32 D_8011AAE0;  /* difficulty: 8, 16 or 32 */

/* Initial health and level of an actor from its definition, scaled by difficulty. */
void func_002427E0(Dest *dst, CIntelligence *def, Vec3 pos, void *actor) {
    Settings *settings;
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
        settings = &D_8011AAD8;
        level = def->m_nTexture;
        if (settings->multiplayer != 0 && def->m_Type != 1 && def->m_Type != 11) {
            if (settings->useLevelB != 0) {
                level = settings->levelB;
            } else {
                level = settings->levelA;
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
            switch (D_8011AAE0) {
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
