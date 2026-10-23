/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030.CPP, func_80444C34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
/* Turok 2: player options pointer at 0x518 (Rage Wars 0x5D8). */
struct OwnerRecAt518 {
    char pad[0x518];
    struct PlayerOptions *settings;
};
/* Turok 2: this toggle is at 0x31 (Rage Wars 0x82). */
struct PlayerOptions {
    char pad0[0x31];
    u8 flag31;
};
extern struct PlayerOptions D_8011AB18;
struct ActorRecAt1C {
    char pad[0x1C];
    struct OwnerRecAt518 *owner;
};
s32 func_0041C764(void *unused, struct ActorRecAt1C *holder) {
    struct PlayerOptions *settings = &D_8011AB18;
    if (holder->owner != 0) {
        settings = holder->owner->settings;
    }
    if (settings->flag31 == 1) {
        settings->flag31 = 0;
    } else {
        settings->flag31 = 1;
    }
    return 0;
}
