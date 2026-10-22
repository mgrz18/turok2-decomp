/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00206604.c, func_00206604.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
/* Turok 2 layout (Rage Wars: typeId 0xE4, flags 0x100). */
typedef struct B01Actor {
    u8 pad0[0xB8];
    u16 typeId;             /* 0xB8 */
    u8 padBA[0xD4 - 0xBA];
    u32 flags;              /* 0xD4 */
} B01Actor;
/* Turok 2 layout (Rage Wars: fn108 0x108, fn10C 0x10C). */
typedef struct B01State {
    u8 pad0[0x2C];
    void *table;            /* 0x2C */
    u8 pad30[0xF0 - 0x30];
    void (*fnF0)();         /* 0xF0 */
    void (*fnF4)();         /* 0xF4 */
} B01State;
typedef struct { u8 p[0x1D]; u8 f; } GFx;
extern GFx D_8011AAD4;
extern u8 D_800B2510[];
void D_00266814(B01Actor *actor, B01State *state);
void D_00266AB4(B01Actor *actor, B01State *state);
s32 func_00284408(void);
void func_002666F4(B01Actor *actor, B01State *state) {
    state->table = D_800B2510;
    state->fnF0 = D_00266814;
    actor->flags |= 0x1000000;
    actor->flags |= 0x2000000;
    actor->flags |= 0x20000;
    if (D_8011AAD4.f == 0) {
        actor->flags |= 0x10000000;
    }
    if (actor->typeId == 0x453 && func_00284408() == 0x6F) {
        state->fnF4 = D_00266AB4;
    }
}
