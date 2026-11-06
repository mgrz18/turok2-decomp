/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388.CPP, func_80217388_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

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
extern void *LoopSounds_Get(void);
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
