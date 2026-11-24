#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "structs/level_actor.h"
#include "structs/sound.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388.CPP, func_80217388_de.
 * The same code in Turok 2, on Turok 2's types (one body for the N64 and the port since round 29):
 * stops the AI's looping sound and every sound the object owns (a static object, type 0, owns its
 * sounds through its word at 0x80, see AI_StartSound). */
void AI_StopSound(CGameObjectInstance *obj, CAI *ai) {
    sptr owner = 0;

    switch (obj->header.m_Type) {
    case 1:
    case 2:
        owner = (sptr) obj;
        break;
    case 0:
        owner = ((CStaticObject *) obj)->m_LitKey;
        break;
    }
    if (ai->m_pAttachedLoopingSound != NULL) {
        Sound__StopOwner(owner);
        if (ai->m_pAttachedLoopingSound != NULL) {
            LoopSounds_Stop(LoopSounds_Get(), ai->m_pAttachedLoopingSound);
            ai->m_pAttachedLoopingSound = NULL;
        }
    }
}
