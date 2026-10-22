/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804443C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct OwnerRecAt5D8V2;
struct PlayerOptions;
struct OwnerRecAt5D8V2 {
    char pad[0x518];
    struct PlayerOptions *settings;
};
struct PlayerOptions;
struct PlayerOptions {
    char pad0[0x2C];
    u8 option2C;
};
extern struct PlayerOptions D_8011AB18;
struct ActorRecAt1C;
struct OwnerRecAt5D8V2;
struct ActorRecAt1C {
    char pad[0x1C];
    struct OwnerRecAt5D8V2 *owner;
};
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_0041BFD0(void *first, struct ActorRecAt1C *holder) {
    struct PlayerOptions *settings = &D_8011AB18;
    if (holder->owner != 0 && holder->owner->settings != 0) {
        settings = holder->owner->settings;
    }
    settings->option2C = func_0041648C(holder, settings->option2C, 1, 0, 1, 1);
    return 0;
}
