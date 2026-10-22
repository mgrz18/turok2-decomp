/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E120.c, func_0022E120.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;

extern u8 D_800F7078[];
void *func_0022425C(void *tbl, s32 id);

typedef struct Actor {
    u8 pad0[0x10];
    s32 typeId; /* 0x10 in Turok 2 (0x14 in Rage Wars) */
} Actor;

s32 func_002567B8(Actor *actor)
{
    s32 id = actor->typeId;
    if (id != 0) {
        void *p = func_0022425C(D_800F7078, id);
        if (p != ((void *)0)) {
            if (*(u16 *)((u8 *)p + 0x52) & 0x40) {
                return 1;
            }
        }
    }
    return 0;
}
