/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct OwnerRecAt5D8V2;
struct PlayerOptions;
struct OwnerRecAt5D8V2 {
    char pad[0x518]; /* Turok 2 offset */
    struct PlayerOptions *settings;
};
struct Field;
struct Field {
    char pad[0x14];
    char **text;
    char pad18[0x4];
    signed int value;
};
struct PlayerOptions;
struct PlayerOptions {
    char pad0[0x2D]; /* Turok 2 layout differs from Rage Wars (0x7B) */
    u8 option2D;
};
extern struct PlayerOptions D_8011AB18;
struct ActorRecAt1C;
struct OwnerRecAt5D8V2;
struct ActorRecAt1C {
    char pad[0x1C];
    struct OwnerRecAt5D8V2 *owner;
};
extern char *D_800B3F48;
extern char *D_800B3F30;
s32 func_0041C0F4(struct Field *field, struct ActorRecAt1C *holder) {
    struct PlayerOptions *settings = &D_8011AB18;
    if (holder->owner != 0 && holder->owner->settings != 0) {
        settings = holder->owner->settings;
    }
    switch (settings->option2D) {
    case 0:
        field->text = &D_800B3F48;
        break;
    case 1:
        field->text = &D_800B3F30;
        break;
    }
    return 0;
}
