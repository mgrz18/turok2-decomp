#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"
#include "structs/sound.h"
#include "protos/game.h"

extern void Sound__StopOwner(void *);
extern s32 LoopSounds_Stop(struct LoopSounds *, SndSource *);

/* host body (round 8 w2): the AI's looping sound by member (the N64 text below reads and clears
 * the N64 CAI 0xEC: on the host that wrote 0 into the CAI at every enemy death). */
void AI_StopSound(CInstanceHeader *arg0, CAI *arg1) {
    void *owner = NULL;

    switch (arg0->m_Type) {
    case 1:
    case 2:
        owner = arg0;
        break;
    }
    if (arg1->m_pAttachedLoopingSound != NULL) {
        Sound__StopOwner(owner);
        if (arg1->m_pAttachedLoopingSound != NULL) {
            LoopSounds_Stop(LoopSounds_Get(), (SndSource *) arg1->m_pAttachedLoopingSound);
            arg1->m_pAttachedLoopingSound = NULL;
        }
    }
}
#else
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388.CPP, func_80217388_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


#include "protos/game.h"
typedef struct SoundSourceNode SoundSourceNode;

/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct LevelActor {
    u8 type;                 /* 0x00: 0 = actor, 1/2 = owner itself */
    u8 pad1[0x80 - 1];
    s32 resourceFlags;       /* 0x80 (0xD0) */
} LevelActor;

typedef struct Dest {
    u8 pad0[0xEC];
    SoundSourceNode *sound;  /* 0xEC (0xFC) */
} Dest;

extern void Sound__StopOwner(void *);
extern s32 LoopSounds_Stop(void *, void *);

void AI_StopSound(void *arg0, Dest *arg1) {
    void *owner = 0;
    switch (((LevelActor *)arg0)->type) {
    case 1:
    case 2:
        owner = arg0;
        break;
    case 0:
        owner = (void *)((LevelActor *)arg0)->resourceFlags;
        break;
    }
    if (arg1->sound != 0) {
        Sound__StopOwner(owner);
        if (arg1->sound != 0) {
            LoopSounds_Stop(LoopSounds_Get(), arg1->sound);
            arg1->sound = 0;
        }
    }
}
#endif
