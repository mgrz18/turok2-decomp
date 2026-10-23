/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/CAMERA/80243A80_11.CPP, func_80245D14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* NOTE: Turok 2 differs from the Rage Wars getter: it stores the current menu
 * context pointer into the object at 0x118. The empty asm keeps the store out of
 * the jr delay slot, as in the ROM. */

typedef unsigned char u8;

typedef struct Shared_MenuContext Shared_MenuContext;
extern Shared_MenuContext *D_80130A50; /* current menu context */

typedef struct {
    u8 pad0[0x118];
    Shared_MenuContext *menuContext; /* 0x118 */
} MenuContextHolder;

void func_002883CC(MenuContextHolder *arg0) {
    arg0->menuContext = D_80130A50;
    __asm__ __volatile__("");
}
