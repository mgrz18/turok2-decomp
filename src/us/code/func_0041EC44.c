/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_7.c, func_804070F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct ObjRecAt5DC;
typedef struct ObjRecAt5DC ObjRecAt5DC;
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct ObjRecAt5DC;
struct ObjRecAt5DC {
    char pad0[0x5DC];
    char * unk5DC;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
struct Shared_PakMenu;
struct PakMenuSlot;
struct PakNotesMenuItem {
    char pad0[8];
    u32 flags;
    char padC[0x1C];
};
typedef struct Shared_PakMenu {
    s16 state;
    char pad02[0xA];
    struct PakNotesMenuItem *items;
    char pad10[4];
    char *dialogData;
    char pad18[4];
    ObjRecAt5DC *player;
    union {
        PakMenuSlot *owner;
        PakMenuSlot *slot;
    };
    void *unk24;
} Shared_PakMenu;
void func_0041DA28(Shared_PakMenu *menu);
extern s32 D_80130994;
extern s32 D_801309BC;
extern s32 D_80130984;
extern s32 D_801309B4;
extern s32 D_801309A0;
extern s32 D_800C2024;
extern s32 D_80130990;
extern s32 D_800C2020;
extern char D_80119E2C[];
extern char D_00439190[];
extern unsigned char D_0043B6F0[];
extern char D_0043BC6C[];
extern char D_0043C05C[];
extern char D_0043BC00[];
extern void func_00426480(s32 ch);
extern s32 func_0042655C(s32 ch);
extern s32 func_00426594(s32 ch);
extern s32 func_00426BD8(s32 ch);
extern void func_00416644(char *, char *, ObjRecAt5DC *, PakMenuSlot *, char *);
void func_0041EC44(Shared_PakMenu *menu)
{
  Shared_PakMenu *new_var3;
  int new_var;
  char *new_var2;
  s32 ch;
  new_var = 1;
  D_800C2024 = 2;
  D_80130984 = 0;
  D_801309A0 = 0;
  D_80130994 = new_var;
  D_801309B4 = 0;
  D_801309BC = 0;
  D_80130990 = 0;
  ch = menu->slot->unk4;
  if (D_800C2020 != 0)
  {
    D_801309BC = 1;
    func_00416644(D_80119E2C, D_00439190, menu->player, menu->slot, 0);
    return;
  }
  menu->items[20].flags &= ~0x01000000;
  func_00426480(menu->slot->unk4);
  new_var = 0;
  if (func_0042655C(ch) == new_var)
  {
    func_0041DA28(menu);
    return;
  }
  D_801309BC = 1;
  if (func_00426594(ch) != new_var)
  {
    new_var2 = D_0043BC6C;
    func_00416644(D_80119E2C, new_var2, menu->player, menu->slot, (char *) 1);
  }
  else
  {
    new_var3 = menu;
    if (func_00426BD8(ch) != 0)
    {
      func_00416644(D_80119E2C, D_0043C05C, new_var3->player, menu->slot, D_0043B6F0);
    }
    else
    {
      func_00416644(D_80119E2C, D_0043BC00, new_var3->player, new_var3->slot, D_00439190);
    }
  }
}
