/* t2loader.c: the engine's cartridge reads on the host (docs/assets/PORT.md 10.4).
 *
 * The N64 loader DMAs ROM bytes in 0x2000-byte chunks (CLoader__Main, osPiRawStartDma) and the
 * decompressor unpacks RNC streams afterwards. On the host CLoader__LoadSync / CLoader__Main
 * (src/us/code/func_00201DBC.c / func_00201FBC.c under TARGET_PORT) call T2Port_ReadAsset with
 * the whole request: T2Asset_ReadInto serves it from the archive mounted by the app (whole units,
 * RNC already unpacked, swapped to host order by t2/swap), and CCache__Request sizes its blocks
 * with T2Port_AssetHostSize. A request that no unit serves is a bug in the unit manifest
 * (tools/assets/romfs.py): it aborts with the address. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t2asset.h"
#include "t2port.h"

/* The PI decodes a 29-bit physical address: the game keeps flags in a key's top bits and the
 * N64 DMA ignores them. CParticleSystem__Spawn ORs the draw layer into a texture key
 * (`*bits |= layer << 30`): layer 2 (a model's translucent particles) requests 0x81AE935C for
 * the ROM block 0x01AE935C (STABILITY.md S16: the loader aborted on it). */
#define T2_PI_ADDR(a) ((a) & 0x1FFFFFFFu)

void T2Port_ReadAsset(u32 address, void *dst, u32 len) {
    address = T2_PI_ADDR(address);
    if (T2Asset_ReadInto(address, 0, dst, len) < 0) {
        fprintf(stderr, "t2: cartridge read %08X+%X: %s (backend %s)\n", address, len, T2Asset_Error(),
                T2Asset_BackendName());
        abort();
    }
}

u32 T2Port_AssetHostSize(u32 key, u32 size) {
    return T2Asset_HostSize(T2_PI_ADDR(key), size);
}

/* VM_Alloc (t2boot.c): the N64 pages a ROM window in through the TLB (the audio control block
 * 0x1E0000 and the sound data file 0xBE1814, both VM_WINDOW units). The host has no pager: the
 * window is the whole unit, resident for the run, in host byte order. Copied into game memory
 * (below 2 GB in low-memory mode: the game keeps the address as s32). */
void *T2Port_AssetWindow(u32 romAddr, u32 size) {
    u32 n = size;
    const u8 *p = (const u8 *)T2Asset_Read(romAddr, &n);
    u8 *w;
    if (p == NULL) {
        fprintf(stderr, "t2: VM window %08X+%X: %s\n", romAddr, size, T2Asset_Error());
        abort();
    }
    w = (u8 *)t2_lowmem_alloc((size_t)(n > size ? n : size) + 16);
    memcpy(w, p, n);
    T2Asset_Release(p);
    return w;
}
