/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_13.c, func_80409144_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
extern char D_0043BDB0[];
struct MenuRecAt24;
typedef struct MenuRecAt24 MenuRecAt24;
struct Messages;
struct Messages {
    char pad0[0x554];
    char text[1];
};
struct Messages;
struct PlayerRecAt5EC;
struct PlayerRecAt5EC {
    char pad0[0x5DC];
    struct Messages *messages;
    char pad5E0[0xC];
    s32 menu;
};
extern s32 D_801309BC;
struct MenuRecAt24;
struct PlayerRecAt5EC;
struct MenuRecAt24 {
    char pad0[0x1C];
    struct PlayerRecAt5EC *player;
    PakMenuSlot *slot;
    char *prompt;
};
struct MenuRecAt24;
struct MenuRecAt24;
struct MenuRecAt24;
extern int func_00420C30(void * owner, struct MenuRecAt24 * menu, void * arg);
struct PlayerRecAt5EC;
typedef struct PlayerRecAt5EC PlayerRecAt5EC;
extern int D_801309C4;
extern char D_0043BE1C[];
extern s32 D_800C2028;
extern s32 D_8011B114;
struct PakMenuSlot;
extern s32 D_80130990;
extern char D_80119E2C[];
extern s32 func_00426064(s32 ch, s32 index);
extern void func_00426480(s32 ch);
extern s32 D_801309B0;
extern s32 D_80130988;
extern char D_0043BDD4[];
extern char D_0043C038[];
extern s32 func_00426814(s32 ch);
extern s32 func_0041F0C8(void *owner, MenuRecAt24 *menu, void *arg);
s32 func_00420C30(void *owner, MenuRecAt24 *menu, void *arg) {
    extern void func_00416644(char *, char *, void *, PakMenuSlot *, char *);
    extern s32 func_0041DD90(MenuRecAt24 *menu, s32 ch, s32 quiet);
    s32 ch;
    switch (D_801309B0) {
        case 4:
            if (D_80130990 != 0) {
                ch = D_800C2028;
            } else {
                ch = menu->slot->unk4;
            }
            if (func_0041DD90(menu, ch, 1) != 0) {
                D_801309BC = 1;
                func_00416644(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            func_00426480(ch);
            if (func_00426064(ch, D_80130988) == 0) {
                func_00416644(D_80119E2C, D_0043BDB0, menu->player, menu->slot, menu->prompt);
            } else {
                func_00416644(D_80119E2C, D_0043BE1C, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 3:
            if (D_80130990 != 0) {
                ch = D_800C2028;
            } else {
                ch = menu->slot->unk4;
            }
            if (func_0041DD90(menu, ch, 1) != 0) {
                D_801309BC = 1;
                func_00416644(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            func_00426480(ch);
            if (func_00426064(ch, D_801309C4) == 0) {
                func_0041F0C8(owner, menu, arg);
                return 1;
            }
            func_00416644(D_80119E2C, D_0043BE1C, menu->player, menu->slot, menu->prompt);
            break;
        case 0:
            if (D_80130990 != 0) {
                ch = D_800C2028;
            } else {
                ch = menu->slot->unk4;
            }
            if (func_0041DD90(menu, ch, 1) != 0) {
                D_801309BC = 1;
                func_00416644(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            func_00426480(ch);
            if (func_00426814(ch) == 0) {
                func_00416644(D_80119E2C, D_0043BDD4, menu->player, menu->slot, menu->prompt);
            } else {
                func_00416644(D_80119E2C, D_0043C038, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 1:
        case 2:
            if (D_80130990 != 0) {
                D_8011B114 = 1;
            }
            func_00416644(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
            break;
    }
    return 1;
}
