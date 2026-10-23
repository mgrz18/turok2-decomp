/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_2/802661FC_2.CPP, func_80267D70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;

typedef struct CVector3 {
    f32 x, y, z;
} CVector3;

typedef struct Player {
    char pad0[0xB18];
    CVector3 requestedPosition;  /* 0xB18 (Rage Wars 0x774) */
} Player;

typedef struct LevelActor {
    char pad0[0x140];
    s32 behaviorFlags;           /* 0x140 (Rage Wars 0x170) */
    char pad144[0x1A8 - 0x144];
    Player *controller;          /* 0x1A8 (Rage Wars 0x1D8) */
} LevelActor;

extern void func_00258734(void *actor, s32 flagsAddr, s32 arg2);

void func_00238914(s32 arg0, void *arg1, s32 arg2, CVector3 arg3, s32 arg6)
{
    s32 t6 = arg6;
    Player *player = ((LevelActor *)arg1)->controller;
    player->requestedPosition = arg3;
    do { } while (0);
    func_00258734(arg1, (s32)&((LevelActor *)arg1)->behaviorFlags, t6);
}
