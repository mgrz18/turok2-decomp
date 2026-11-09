/* t2pif.c: the virtual PIF behind __osSiRawStartDma (docs/port/INPUT.md).
 *
 * The game keeps libultra's Controller Pak C (osPfsInitPak, osPfsIsPlug, osPfsChecker,
 * osPfsFindFile, osPfsReadWriteFile, osPfsAllocateFile, ...): it builds joybus commands in
 * __osPfsPifRam, starts an SI DMA to the PIF, waits for OS_EVENT_SI, DMAs the PIF RAM back and
 * reads the answers. This file is the PIF and the controllers on the other side of that DMA:
 *
 *   write (direction 1): the 64 bytes are copied in and every channel's command runs at once;
 *   read  (direction 0): the answered 64 bytes are copied back.
 *   Each DMA posts OS_EVENT_SI (the game's queue is gSiMessageQueue, the SI event queue).
 *
 * Joybus commands per channel 0-3 (a controller is connected when the app's ControlDeck
 * reports it, t2ultra.c):
 *   0x00 / 0xFF  status / reset: type 0x0005, status bit 0 = a pak is in the slot
 *   0x01         read buttons (the game reads pads through osContGetReadData; zeros here)
 *   0x02         pak read: 32 bytes at (address & 0xFFE0) + data CRC
 *   0x03         pak write: 32 bytes, answers the data CRC
 * A pak address CRC that does not match is logged; addresses >= 0x8000 (bank select, rumble
 * probe) read zeros and ignore writes, as a 32 KB Controller Pak does. With no pak in the slot
 * the controller still answers, with the data CRC inverted (libultra then asks the status and
 * returns PFS_ERR_NOPACK).
 *
 * The pak image is the emulator-standard raw 32 KB .mpk (big-endian, the console's layout), one
 * file per port: <pak dir>/turok2_pak<N>.mpk, N = 1..4. A missing file is a blank formatted pak
 * (created on the first write). Every write goes to the file at once.
 *
 * Byte order: the game's pfs C reads the pak's structured blocks with native loads (the pack ID's
 * u16 checksum words, the directory's u32 game code ...). The PIF converts those blocks between
 * the file's big-endian layout and host order as they cross the DMA, so the game's C runs
 * unchanged and the file stays the standard one:
 *   - ID blocks (1, 3, 4, 6): __OSPackId fields swapped; the checksum pair is recomputed for the
 *     other byte order when it was valid (a damaged ID stays damaged);
 *   - inode blocks (pages 1-2: blocks 8-23): every __OSInodeUnit swapped as the u16 libultra also
 *     reads it as (`ipage`); on a little-endian host include/ultra_types.h orders the unit's bytes
 *     {page, bank} so that `inode_t.bank`/`.page` and `ipage` agree as on the console;
 *   - directory blocks (pages 3-4: blocks 24-39, one __OSDir each for a 1-bank pak): game_code,
 *     company_code, start_page, data_sum swapped;
 *   - everything else (label, inode tables, note pages) is bytes.
 * Note data itself is whatever the game writes (its save structures in host order today: not
 * interchangeable with an emulator's saves until the save records get a swap plan).
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define T2_RENAME_NO_LIBC
#include "t2_rename.h"
#include "t2os.h"
#include "t2port.h"

/* t2pif.c */
void t2pif_configure(const T2PortConfig *cfg, u8 (*contBits)(void));
int t2pif_pak_present(int ch);
void t2pif_get_stats(T2PifStats *out);

#define PAK_SIZE 0x8000
#define PAK_BLOCK 32
#define PIF_RAM 64

typedef struct {
    int present;          /* a pak is in the slot */
    int loaded;           /* image initialized (file read or blank formatted) */
    int dirty;            /* image differs from the file (volatile mode keeps it so) */
    u8 image[PAK_SIZE];
} T2Pak;

static T2Pak sPaks[4];
static int sPakMode = T2_PAK_FILE;
static char sPakDir[1024];
static u8 sPif[PIF_RAM];
static u8 (*sContBitsFn)(void);
static T2PifStats sPifStats;
static int sPifLog = -1; /* T2_PIF_LOG=1: log every pak block read/write (stderr) */

static void pif_log(const char *what, int ch, u16 addr, const u8 *blk) {
    if (sPifLog < 0)
        sPifLog = getenv("T2_PIF_LOG") != NULL;
    if (!sPifLog)
        return;
    fprintf(stderr, "t2: PIF %s ch%d %04X:", what, ch, addr);
    for (int i = 0; i < PAK_BLOCK; i++)
        fprintf(stderr, " %02X", blk[i]);
    fprintf(stderr, "\n");
}

/* ---- CRCs (libultra's __osContAddressCrc / __osContDataCrc) ---- */

static u8 addr_crc(u16 addr) {
    u32 a = addr;
    u32 r = 0;
    for (int i = 0; i < 16; i++) {
        u32 x = (r & 0x10) ? 0x15 : 0;
        r <<= 1;
        r |= (a >> 10) & 1;
        a <<= 1;
        r = (r & 0xFF) ^ x;
    }
    return (u8)(r & 0x1F);
}

static u8 data_crc(const u8 *data) {
    u8 r = 0;
    for (int i = 0; i <= 32; i++) {
        for (int j = 7; j >= 0; j--) {
            u8 x = (r & 0x80) ? 0x85 : 0;
            r <<= 1;
            if (i < 32 && (data[i] & (1 << j)))
                r |= 1;
            r ^= x;
        }
    }
    return r;
}

/* ---- the standard image ---- */

static u16 be16(const u8 *p) { return (u16)(p[0] << 8 | p[1]); }
static void put_be16(u8 *p, u16 v) { p[0] = (u8)(v >> 8); p[1] = (u8)v; }

static int host_is_le(void) {
    const u16 one = 1;
    return *(const u8 *)&one == 1;
}

/* __osIdCheckSum over 14 u16 words read in big-endian (be = 1) or host (be = 0) order */
static void id_sum(const u8 *id, int be, u16 *sum, u16 *isum) {
    u16 s = 0, is = 0;
    for (int j = 0; j < 28; j += 2) {
        u16 w;
        if (be)
            w = be16(id + j);
        else
            memcpy(&w, id + j, 2);
        s += w;
        is += (u16)~w;
    }
    *sum = s;
    *isum = is;
}

static void swap_bytes(u8 *p, int n) {
    for (int i = 0; i < n / 2; i++) {
        u8 t = p[i];
        p[i] = p[n - 1 - i];
        p[n - 1 - i] = t;
    }
}

/* one __OSPackId between the file (big-endian) and host order */
static void id_convert(u8 *id, int toHost) {
    u16 sum, isum, fs, fis;
    id_sum(id, toHost, &sum, &isum); /* in the source order */
    if (toHost) {
        fs = be16(id + 0x1C);
        fis = be16(id + 0x1E);
    } else {
        memcpy(&fs, id + 0x1C, 2);
        memcpy(&fis, id + 0x1E, 2);
    }
    int valid = fs == sum && fis == isum;
    swap_bytes(id + 0x00, 4); /* repaired */
    swap_bytes(id + 0x04, 4); /* random */
    swap_bytes(id + 0x08, 8); /* serial_mid */
    swap_bytes(id + 0x10, 8); /* serial_low */
    swap_bytes(id + 0x18, 2); /* deviceid */
    swap_bytes(id + 0x1C, 2); /* checksum */
    swap_bytes(id + 0x1E, 2); /* inverted_checksum */
    if (valid) {
        id_sum(id, !toHost, &sum, &isum); /* in the destination order */
        if (toHost) {
            memcpy(id + 0x1C, &sum, 2);
            memcpy(id + 0x1E, &isum, 2);
        } else {
            put_be16(id + 0x1C, sum);
            put_be16(id + 0x1E, isum);
        }
    }
}

/* one __OSDir */
static void dir_convert(u8 *d) {
    swap_bytes(d + 0x00, 4); /* game_code */
    swap_bytes(d + 0x04, 2); /* company_code */
    swap_bytes(d + 0x06, 2); /* start_page (__OSInodeUnit) */
    swap_bytes(d + 0x0A, 2); /* data_sum */
}

static void block_convert(u16 addr, u8 *blk, int toHost) {
    if (!host_is_le())
        return;
    u32 b = addr / PAK_BLOCK;
    if (b == 1 || b == 3 || b == 4 || b == 6) {
        id_convert(blk, toHost);
    } else if (b >= 8 && b < 24) { /* inode table and its mirror: 16 __OSInodeUnit per block */
        for (int i = 0; i < PAK_BLOCK; i += 2)
            swap_bytes(blk + i, 2);
    } else if (b >= 24 && b < 40) {
        dir_convert(blk);
    }
}

/* a blank formatted 1-bank pak (libultra's layout; the label and ID as emulators write them) */
static void format_pak(u8 *img) {
    memset(img, 0, PAK_SIZE);
    /* block 0: label area */
    img[0] = 0x81;
    for (int i = 1; i < 32; i++)
        img[i] = (u8)i;
    /* the pack ID, big-endian */
    u8 id[32];
    memset(id, 0, sizeof(id));
    memset(id + 0x00, 0xFF, 4);                  /* repaired */
    id[4] = 0x05, id[5] = 0x1A, id[6] = 0x5F, id[7] = 0x13; /* random */
    memset(id + 0x10, 0xFF, 8);                  /* serial_low */
    put_be16(id + 0x18, 0x0001);                 /* deviceid: bit 0 = formatted */
    id[0x1A] = 1;                                /* banks */
    id[0x1B] = 0;                                /* version */
    u16 sum, isum;
    id_sum(id, 1, &sum, &isum);
    put_be16(id + 0x1C, sum);
    put_be16(id + 0x1E, isum);
    static const int idBlocks[] = { 1, 3, 4, 6 };
    for (int i = 0; i < 4; i++)
        memcpy(img + idBlocks[i] * PAK_BLOCK, id, 32);
    /* pages 1 and 2: the inode table and its backup. Pages 0-4 are the system area
     * (inode_start_page = banks * 2 + 3 = 5); entry 0's page byte is the checksum of entries
     * 5..127; free pages are 3 (PFS_PAGE_NOT_USED) */
    for (int page = 1; page <= 2; page++) {
        u8 *ino = img + page * 0x100;
        u32 s = 0;
        for (int e = 1; e < 128; e++) {
            ino[e * 2] = 0;
            ino[e * 2 + 1] = 3;
            if (e >= 5)
                s += 3;
        }
        ino[0] = 0;
        ino[1] = (u8)s;
    }
    /* pages 3-4: the directory, 16 empty entries; the note pages stay zero */
}

static void pak_path(int ch, char *out, size_t n) {
    snprintf(out, n, "%s/turok2_pak%d.mpk", sPakDir, ch + 1);
}

static void mkdirs(const char *dir) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", dir);
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    mkdir(tmp, 0755);
}

static void pak_load(int ch) {
    T2Pak *pak = &sPaks[ch];
    if (pak->loaded)
        return;
    pak->loaded = 1;
    if (sPakMode == T2_PAK_FILE) {
        char path[1100];
        pak_path(ch, path, sizeof(path));
        FILE *f = fopen(path, "rb");
        if (f != NULL) {
            size_t got = fread(pak->image, 1, PAK_SIZE, f);
            fclose(f);
            if (got == PAK_SIZE) {
                fprintf(stderr, "t2: Controller Pak %d: %s\n", ch + 1, path);
                return;
            }
            fprintf(stderr, "t2: Controller Pak %d: %s is %zu bytes, not 32768: using a blank pak\n", ch + 1,
                    path, got);
        } else {
            fprintf(stderr, "t2: Controller Pak %d: %s not found: blank formatted pak (written on the first save)\n",
                    ch + 1, path);
        }
    } else {
        fprintf(stderr, "t2: Controller Pak %d: blank formatted pak in memory (not saved)\n", ch + 1);
    }
    format_pak(pak->image);
}

static void pak_flush(int ch, u16 addr) {
    if (sPakMode != T2_PAK_FILE)
        return;
    char path[1100];
    pak_path(ch, path, sizeof(path));
    FILE *f = fopen(path, "r+b");
    if (f == NULL) {
        mkdirs(sPakDir);
        f = fopen(path, "w+b");
        if (f == NULL) {
            fprintf(stderr, "t2: Controller Pak %d: cannot write %s: %s\n", ch + 1, path, strerror(errno));
            return;
        }
        fwrite(sPaks[ch].image, 1, PAK_SIZE, f); /* the whole image the first time */
    } else {
        fseek(f, addr, SEEK_SET);
        fwrite(sPaks[ch].image + addr, 1, PAK_BLOCK, f);
    }
    fclose(f);
}

/* ---- configuration ---- */

void t2pif_configure(const T2PortConfig *cfg, u8 (*contBits)(void)) {
    sContBitsFn = contBits;
    sPakMode = cfg->pakMode;
    memset(sPaks, 0, sizeof(sPaks));
    if (cfg->pakDir != NULL && cfg->pakDir[0] != 0) {
        snprintf(sPakDir, sizeof(sPakDir), "%s", cfg->pakDir);
    } else {
        const char *xdg = getenv("XDG_DATA_HOME");
        const char *home = getenv("HOME");
#ifdef __APPLE__
        snprintf(sPakDir, sizeof(sPakDir), "%s/Library/Application Support/turok2", home ? home : ".");
#else
        if (xdg != NULL && xdg[0] == '/')
            snprintf(sPakDir, sizeof(sPakDir), "%s/turok2", xdg);
        else
            snprintf(sPakDir, sizeof(sPakDir), "%s/.local/share/turok2", home ? home : ".");
#endif
        (void)xdg;
    }
    int ports = cfg->pakPorts != 0 ? cfg->pakPorts : 1;
    for (int i = 0; i < 4; i++)
        sPaks[i].present = sPakMode != T2_PAK_NONE && (ports & (1 << i)) != 0;
}

int t2pif_pak_present(int ch) {
    if (ch < 0 || ch >= 4 || sContBitsFn == NULL || !(sContBitsFn() & (1 << ch)))
        return 0;
    return sPaks[ch].present;
}

void t2pif_get_stats(T2PifStats *out) {
    *out = sPifStats;
}

/* ---- the joybus commands ---- */

static void run_command(int ch, u8 *rxByte, int tx, int rx, const u8 *cmd, u8 *resp) {
    int connected = sContBitsFn != NULL && (sContBitsFn() & (1 << ch)) != 0;
    if (!connected) {
        *rxByte |= 0x80; /* no response */
        return;
    }
    int pak = t2pif_pak_present(ch);
    switch (cmd[0]) {
    case 0x00: /* status */
    case 0xFF: /* reset */
        if (rx >= 3) {
            resp[0] = 0x05; /* CONT_TYPE_NORMAL, as libultra reads it (typeh, typel) */
            resp[1] = 0x00;
            resp[2] = pak ? 0x01 : 0x00; /* CONT_CARD_ON */
        }
        sPifStats.status++;
        break;
    case 0x01: /* read buttons */
        if (rx >= 4)
            memset(resp, 0, 4);
        break;
    case 0x02: { /* pak read */
        if (tx < 3 || rx < 33)
            break;
        u16 a = (u16)(cmd[1] << 8 | cmd[2]);
        u16 addr = a & 0xFFE0;
        if (addr_crc(addr >> 5) != (a & 0x1F))
            t2_log_once("PIF: pak read with a bad address CRC");
        if (pak && addr < PAK_SIZE) {
            pak_load(ch);
            memcpy(resp, sPaks[ch].image + addr, PAK_BLOCK);
            block_convert(addr, resp, 1);
        } else {
            memset(resp, 0, PAK_BLOCK);
        }
        pif_log("read ", ch, addr, resp);
        resp[32] = data_crc(resp);
        if (!pak)
            resp[32] ^= 0xFF;
        sPifStats.reads++;
        break;
    }
    case 0x03: { /* pak write */
        if (tx < 35 || rx < 1)
            break;
        u16 a = (u16)(cmd[1] << 8 | cmd[2]);
        u16 addr = a & 0xFFE0;
        if (addr_crc(addr >> 5) != (a & 0x1F))
            t2_log_once("PIF: pak write with a bad address CRC");
        const u8 *data = cmd + 3;
        pif_log("write", ch, addr, data);
        if (pak && addr < PAK_SIZE) {
            pak_load(ch);
            u8 blk[PAK_BLOCK];
            memcpy(blk, data, PAK_BLOCK);
            block_convert(addr, blk, 0);
            if (memcmp(sPaks[ch].image + addr, blk, PAK_BLOCK) != 0) {
                memcpy(sPaks[ch].image + addr, blk, PAK_BLOCK);
                pak_flush(ch, addr);
            }
        }
        resp[0] = data_crc(data);
        if (!pak)
            resp[0] ^= 0xFF;
        sPifStats.writes++;
        break;
    }
    default:
        *rxByte |= 0x80;
        t2_log_once("PIF: unknown joybus command");
        break;
    }
}

static void pif_run(void) {
    int ch = 0;
    int i = 0;
    while (i < PIF_RAM - 1 && ch < 5) {
        u8 b = sPif[i];
        if (b == 0xFE)
            break;
        if (b == 0xFF || b == 0xFD) { /* padding */
            i++;
            continue;
        }
        if (b == 0x00) { /* skip this channel */
            ch++;
            i++;
            continue;
        }
        int tx = b & 0x3F;
        if (i + 1 >= PIF_RAM - 1)
            break;
        int rx = sPif[i + 1] & 0x3F;
        if (i + 2 + tx + rx > PIF_RAM - 1)
            break; /* not a command (stray byte after the last one) */
        if (ch < 4)
            run_command(ch, &sPif[i + 1], tx, rx, &sPif[i + 2], &sPif[i + 2 + tx]);
        i += 2 + tx + rx;
        ch++;
    }
}

s32 __osSiRawStartDma(s32 direction, void *dramAddr) {
    if (direction == 1) { /* OS_WRITE: RDRAM -> PIF, the PIF runs the commands */
        memcpy(sPif, dramAddr, PIF_RAM);
        pif_run();
        memset(sPif + 0x3C, 0, 4); /* the control word: done */
    } else {
        memcpy(dramAddr, sPif, PIF_RAM);
    }
    sPifStats.dmas++;
    t2os_post_event(OS_EVENT_SI);
    return 0;
}
