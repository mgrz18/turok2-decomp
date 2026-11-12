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

#ifdef TARGET_PORT
#include "structs/boss.h"
#include "globals/boss.h"
/* host body (round 10 y2): gLevelScript by member. The view anchored at found[1] reached bossMode
 * at the N64 delta +0x64; on the host found[] holds 8-byte pointers and bossMode sits further on:
 * `mode = 8` hit another word, bossMode stayed 1 and the Primagen idled in mode 18 for good (its
 * update only acts in boss modes 3, 4, 8 and 9): no attack, no part ever opened. */
void Primagen_UpdateMode6(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(gLevelScript.found[1], &gLevelScript.found[1]->m_AI, 1);
        CAI__SetMode(gLevelScript.found[0], &gLevelScript.found[0]->m_AI, 1);
        gLevelScript.bossMode = 8;
        D_800F1C40[0] |= 0x300;
        D_800F1F50[0] |= 0x300;
        CAI__SetMode(arg0, ai, 0x12);
    }
}
#else
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
#endif
