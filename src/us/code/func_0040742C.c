#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


/* LevelScriptState (structs/boss.h) from found[1] on, D_8012F978 (+0x370):
 * the word before it is found[0], the other actor; mode is bossMode. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *actor;
    /* 0x04 */ u8 pad4[0x64 - 0x4];
    /* 0x64 */ s32 mode;
} LevelScriptF978;

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern LevelScriptF978 D_8012F978;

void Primagen_UpdateMode6(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(D_8012F978.actor, &D_8012F978.actor->m_AI, 1);
        CAI__SetMode((sptr) (&D_8012F978.actor)[-1], (sptr) &(&D_8012F978.actor)[-1]->m_AI, 1);
        D_8012F978.mode = 8;
        D_800F1C40[0] |= 0x300;
        D_800F1F50[0] |= 0x300;
        CAI__SetMode(arg0, ai, 0x12);
    }
}
