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

extern void func_002758B4(void *);
extern void *func_00275F7C(void);
extern s32 func_00275D34(void *, void *);

void func_00246918(void *arg0, Dest *arg1) {
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
        func_002758B4(owner);
        if (arg1->sound != 0) {
            func_00275D34(func_00275F7C(), arg1->sound);
            arg1->sound = 0;
        }
    }
}
