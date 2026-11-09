/* t2asset_game.h: the asset reader as the game's own C sees it (port build only).
 *
 * The engine reads the cartridge by ROM address (CLoader, CCache__Request keys, VM windows). On
 * the host those reads go through port/assets/t2asset.c: whole load units out of turok2.o2r,
 * RNC already unpacked and swapped to host byte order with the archive's t2/swap plans
 * (docs/assets/PORT.md 10.4, ARCHIVE.md, BYTEORDER.md). Implemented in port/runtime/t2loader.c;
 * declared here with the game's own integer types so game files need not see t2asset.h. */
#ifndef T2ASSET_GAME_H
#define T2ASSET_GAME_H

#ifndef TARGET_PORT
#error "t2asset_game.h is for the port build only"
#endif

/* Copy len bytes of the cartridge at `address` (a unit key, or an address inside an audio
 * unit) to dst in host order: T2Asset_ReadInto. Aborts with the reason if nothing serves it. */
void T2Port_ReadAsset(unsigned int address, void *dst, unsigned int len);
/* What CCache__Request must allocate for (key, size): the unpacked size of an RNC unit asked
 * for whole, otherwise size (T2Asset_HostSize). */
unsigned int T2Port_AssetHostSize(unsigned int key, unsigned int size);

#endif
