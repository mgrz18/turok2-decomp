#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "structs/level_actor.h"
#include "structs/sound.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002172D0.c, func_002172D0.
 * The same code in Turok 2, on Turok 2's types: the AI's looping sound is m_pAttachedLoopingSound
 * (a SndSource; its id is SndSource.sound, 0xC). One body for the N64 and the port since round 29
 * (the N64 view read the N64 CAI 0xEC, so the port had its own copy). The owner of a level actor's
 * sounds (type 0, a CStaticObject) is its word at 0x80, m_LitKey, as the ROM reads it. */
void AI_StartSound(CInstanceHeader *src, CAI *owner, s32 id) {
    sptr s0 = -1;

    if (owner->m_pAttachedLoopingSound != NULL) {
        if (((SndSource *) owner->m_pAttachedLoopingSound)->sound.w == id) {
            return;
        }
        LoopSounds_Stop(LoopSounds_Get(), owner->m_pAttachedLoopingSound);
    }
    switch (src->m_Type) {
    case 1:
    case 2:
        s0 = (sptr) src;
        break;
    case 0:
        s0 = ((CStaticObject *) src)->m_LitKey;
        break;
    }
    owner->m_pAttachedLoopingSound = LoopSounds_Start(LoopSounds_Get(), id, &src->m_vPos, &src->m_vPos, s0);
}
