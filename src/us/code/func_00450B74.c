/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84_3.CPP, func_8043DEF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
struct OwnerCode8043DF84;
struct OwnerCode8043DF84 {
    char pad0[0x85C];
    s32 unk85C;
};
struct PakMenuSlot;
struct MenuRecAt20V2;
typedef struct MenuRecAt20V2 MenuRecAt20V2;
struct MenuRecAt20V2;
struct OwnerCode8043DF84;
struct MenuRecAt20V2 {
    char pad0[0x1C];
    struct OwnerCode8043DF84 *owner;
    PakMenuSlot *widget;
};
extern void func_00450B74(void *arg0, s32 arg1);
extern void Controller_WatchPak(s32 arg);
extern void Controller_UnwatchPak(sptr arg);
void func_00450B74(void *arg0, s32 arg1) {
    s32 val;
    MenuRecAt20V2 *menu = arg0;
    val = menu->widget->unk4;
    if (arg1 != 0) {
        Controller_WatchPak(val);
    } else {
        Controller_UnwatchPak(val);
    }
}
