/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EBE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

/* Turok 2 has 4 player-settings records of 0x40 bytes (Rage Wars: 8 of 0x96);
 * the base of the array is a guess, only `enabled` (0xA8 for player 0) is known. */
typedef struct PlayerSettings {
    u8 pad0[0x28];
    u8 enabled; /* 0x28 */
    u8 pad29[0x40 - 0x29];
} PlayerSettings;

typedef struct Shared_Game {
    u8 pad0[0x19];
    u8 local;  /* 0x19 (0x1D in Rage Wars) */
    u8 mode1E; /* 0x1A (0x1E in Rage Wars) */
    u8 pad1B[0x80 - 0x1B];
    PlayerSettings playerSettings[4]; /* 0x80 */
} Shared_Game;

extern Shared_Game D_8011AAD8;
extern char D_00438FB4[];
extern void func_00416894(void *, void *, void *);

s32 func_0045275C(void *first, void *second, void *third) {
    Shared_Game *options = &D_8011AAD8;
    s32 i;
    options->local = 1;
    options->mode1E = 4;
    for (i = 0; i < 4; i++) {
        options->playerSettings[i].enabled = 0;
    }
    func_00416894(third, second, D_00438FB4);
    return 1;
}
