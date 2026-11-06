/* WRONG CUT: REAL Pak_ProbeSlot size 0xDC (splat cut it into Pak_ProbeSlot + func_004264B4 + func_004264D0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_7.CPP, func_80404E28_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/pak.h"
#include "structs/controller.h"

extern void **D_800C2228;
extern s32 D_800C2220;
extern u8 D_800F6750;

extern void **CCache__Allocate(s32, s32, s32, char *);
extern void CCache__Flush(s32);
extern void CCache__Deallocate(s32, void **);
extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 Pak_Mount(s32 ch);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);
extern u8 D_800F6798[];
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);

/* Probes the pak in slot ch and records its state: 2 for a Rumble Pak, otherwise 3 or 1 depending on Pak_Mount. */
void Pak_ProbeSlot(s32 ch) {
    RumblePak *cur;
    s32 ready;

    Controller_Lock(1);
    Controller_ReadAll();
    D_800F6750 = 2;

    cur = &D_80130AA0[ch];
    if (D_800F6778[ch].errno != 0) {
        cur->present = 0;
        ready = 0;
    } else {
        cur->present = osMotorInit(D_800F6798, &cur->pfs, ch) == 0;
        ready = cur->present;
    }

    if (ready != 0) {
        D_80130C60[ch] = 2;
    } else if (Pak_Mount(ch) != 0) {
        D_80130C60[ch] = 3;
    } else {
        D_80130C60[ch] = 1;
    }
    Controller_Unlock();
}
