/* Boundary fixed in versions/cuts.us.txt: splat's func_0028D374 starts with 3 stray words (a jal
 * func_0021E03C / swc1 / addu tail that nothing executes); the real function
 * starts at 0x28D380 (splat's D_0028D380), 0x38C bytes. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A7F58.c,
 * func_802A71C0_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Marker {
    char text[15];
} Marker;

extern Marker D_800AA200;           /* the boot marker string */
extern char D_800B75F0[];           /* where the marker is kept (0x10 bytes) */
extern s32 D_80120690;              /* set when the marker was found: a warm boot */
extern char D_8011F290[];           /* the saved players (0x1400) */
extern char D_8011AAD8[];           /* the settings (0x184), pads at +0x1D8..+0x224 */
extern char D_800EBAC0[];
extern char D_800E6AC0[];
extern s32 D_800C4000[];            /* BSS start */
extern s32 D_8014A000[];            /* BSS end */

void func_00266C5C(void *dst, s32 value, s32 size);
s32 func_00288780(char *marker, char *name);
void func_00288800(char *marker, char *name);
void func_0029B1F0(void);
void func_002881E8(void);

/* Whether a word may be overwritten on reset (func_0028D714 inlined). */
static __inline__ s32 isRestorable(u32 address) {
    if (address >= (u32)D_800B75F0 && address < (u32)D_800B75F0 + 0x10) {
        return 0;
    }
    if (address == (u32)&D_80120690) {
        return 0;
    }
    if (D_80120690 != 0) {
        return 1;
    }
    if (address >= (u32)D_8011F290 && address < (u32)D_8011F290 + 0x1400) {
        return 0;
    }
    if (address >= (u32)D_8011AAD8 && address < (u32)D_8011AAD8 + 0x184) {
        return 0;
    }
    if (address >= (u32)D_800EBAC0 && address < (u32)D_800EBAC0 + 0x5800) {
        return 0;
    }
    if (address >= (u32)D_800E6AC0 && address < (u32)D_800E6AC0 + 0x5000) {
        return 0;
    }
    if (address >= (u32)D_8011AAD8 + 0x1D8 && address <= (u32)D_8011AAD8 + 0x224) {
        return 0;
    }
    return 1;
}

/* Soft reset: checks the boot marker, and on a cold boot copies the resident
 * code and data back from the cartridge word by word; then clears the BSS,
 * in both passes skipping the protected regions. */
s32 func_0028D380(void) {
    u32 *src;
    u32 *dst;
    s32 count;
    u32 status;
    u32 *piStatus;
    char name[16];

    *(Marker *)name = D_800AA200;
    func_00266C5C(&name[15], 0, 1);
    if (func_00288780(D_800B75F0, name) == 0) {
        D_80120690 = 0;
    } else {
        D_80120690 = 1;
        func_00288800(D_800B75F0, name);
    }
    if (D_80120690 == 0) {
        src = (u32 *)0xB0001000;
        dst = (u32 *)0x80000400;
        piStatus = (u32 *)0xA4600010;
        count = 0x3FFFF;
        do {
            status = *piStatus & 3;
            while (status != 0) {
            }
            if (isRestorable((u32)dst | 0x80000000)) {
                *dst = *src;
            }
            dst++;
            src++;
        } while (count-- != 0);
    }
    for (dst = (u32 *)D_800C4000; (u32)dst < (u32)D_8014A000; dst++) {
        if (isRestorable((u32)dst | 0x80000000)) {
            *dst = 0;
        }
    }
    func_0029B1F0();
    func_002881E8();
    return 0;
}
