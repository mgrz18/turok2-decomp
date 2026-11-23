/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_14.c, func_802571F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"
#include "globals/sysrc.h"
#include "globals/audiomgr.h"

extern u32 __amDMA(s32 offset, s32 length);
extern void CLoader__LoadAsync(CLoader *reader, u32, s32, sptr, void *, s32, s32);
u32 __amDMA(s32 offset, s32 length) {
    extern u32 osVirtualToPhysical(sptr);
    extern void alLink(AMDMABuffer *, AMDMABuffer *);
    extern void alUnlink(AMDMABuffer *);
    AMDMABuffer *window;
    AMDMABuffer *after;
    AMDMABuffer **pool;
    u32 address;
    s32 end;
    s32 top;
    s32 odd;
    s32 word;
    after = 0;
    address = offset + gAudioRomBase;
    end = address + length;
    for (window = gAudioDmaState_firstUsed; window != 0; window = (AMDMABuffer *) window->node.next) {
        top = window->startAddr + 0x300;
        word = window->startAddr;
        if (address < word) {
            break;
        }
        after = window;
        if (top >= end) {
            window->lastFrame = audFrameCt;
            return osVirtualToPhysical((sptr) (window->ptr + address - window->startAddr));
        }
    }
    pool = &gAudioDmaState_firstFree;
    window = *pool;
    *pool = (AMDMABuffer *) window->node.next;
    alUnlink(window);
    if (after != 0) {
        alLink(window, after);
    } else if (pool[-1] != 0) {
        after = pool[-1];
        pool[-1] = window;
        window->node.next = (ALLink *) after;
        window->node.prev = 0;
        after->node.prev = (ALLink *) window;
    } else {
        pool[-1] = window;
        window->node.next = 0;
        window->node.prev = 0;
    }
    odd = address & 1;
    address -= odd;
    window->startAddr = address;
    window->lastFrame = audFrameCt;
    word = (sptr) window->ptr;
    nextDMA++;
    CLoader__LoadAsync(&gLoader, address, 0x300, word, &gAudioDmaMessageQ, 0, 1);
    return osVirtualToPhysical(word) + odd;
}
