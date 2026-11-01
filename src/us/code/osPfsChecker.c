/* cc1-flags: -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin */
/* From decompals/ultralib (libultra 2.0I): src/io/pfschecker.c, osPfsChecker. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;

typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    /* 0x00 */ s32 status;
    /* 0x04 */ OSMesgQueue *queue;
    /* 0x08 */ s32 channel;
    /* 0x0C */ u8 id[32];
    /* 0x2C */ u8 label[32];
    /* 0x4C */ s32 version;
    /* 0x50 */ s32 dir_size;
    /* 0x54 */ s32 inode_table;
    /* 0x58 */ s32 minode_table;
    /* 0x5C */ s32 dir_table;
    /* 0x60 */ s32 inode_start_page;
    /* 0x64 */ u8 banks;
    /* 0x65 */ u8 activebank;
} OSPfs;

typedef struct {
    /* 0x00 */ u32 repaired;
    /* 0x04 */ u32 random;
    /* 0x08 */ u64 serial_mid;
    /* 0x10 */ u64 serial_low;
    /* 0x18 */ u16 deviceid;
    /* 0x1A */ u8 banks;
    /* 0x1B */ u8 version;
    /* 0x1C */ u16 checksum;
    /* 0x1E */ u16 inverted_checksum;
} __OSPackId;

typedef signed char s8;

typedef union {
    struct {
        /* 0x0 */ u8 bank;
        /* 0x1 */ u8 page;
    } inode_t;
    u16 ipage;
} __OSInodeUnit;

typedef struct {
    /* 0x00 */ __OSInodeUnit inode_page[128];
} __OSInode;

typedef struct {
    /* 0x00 */ u32 game_code;
    /* 0x04 */ u16 company_code;
    /* 0x06 */ __OSInodeUnit start_page;
    /* 0x08 */ u8 status;
    /* 0x09 */ s8 reserved;
    /* 0x0A */ u16 data_sum;
    /* 0x0C */ u8 ext_name[4];
    /* 0x10 */ u8 game_name[16];
} __OSDir;

typedef struct {
    /* 0x000 */ __OSInode inode;
    /* 0x100 */ u8 bank;
    /* 0x101 */ u8 map[256];
} __OSInodeCache;

#define PFS_READ 0
#define PFS_WRITE 1
#define PFS_ERR_NEW_PACK 2
#define PFS_ERR_INCONSISTENT 3
#define PFS_CORRUPTED 2
#define PFS_EOF 1
#define PFS_PAGE_NOT_USED 3
#define DIR_STATUS_EMPTY 0

#define ERRCK(fn) \
    ret = fn;     \
    if (ret != 0) \
        return ret

#define SET_ACTIVEBANK_TO_ZERO()                 \
    if (pfs->activebank != 0) {                  \
        pfs->activebank = 0;                     \
        ERRCK(func_00429DE4(pfs));               \
    }

#define CHECK_IPAGE(p)                                                                                        \
    (((p).ipage >= pfs->inode_start_page) && ((p).inode_t.bank < pfs->banks) && ((p).inode_t.page >= 0x01) && \
     ((p).inode_t.page < 0x80))

extern s32 func_00429D1C(OSPfs *pfs); /* __osCheckId */
extern s32 func_004297D8(OSPfs *pfs); /* __osGetId */
extern s32 func_00429DE4(OSPfs *pfs); /* __osPfsSelectBank */
extern s32 func_004299C4(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank); /* __osPfsRWInode */
extern s32 func_00299680(OSMesgQueue *mq, int channel, u16 address, u8 *buffer); /* __osContRamRead */
extern s32 func_00299A40(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force); /* __osContRamWrite */
extern s32 func_0042868C(OSPfs *pfs, __OSInodeCache *cache); /* corrupted_init */
extern s32 func_004287D8(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache); /* corrupted */

s32 osPfsChecker(OSPfs *pfs) {
    int j;
    s32 ret;
    __OSInodeUnit next_page;
    __OSInode checked_inode;
    __OSInode tmp_inode;
    __OSDir tmp_dir;
    __OSInodeUnit file_next_node[16];
    __OSInodeCache cache;
    int fixed = 0;
    u8 bank;
    s32 cc;
    s32 cl;
    int offset;

    ret = func_00429D1C(pfs);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = func_004297D8(pfs);
    }

    if (ret != 0) {
        return ret;
    }

    ERRCK(func_0042868C(pfs, &cache));

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(func_00299680(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir));

        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0) {
            next_page = tmp_dir.start_page;
            cl = cc = 0;
            bank = 255;

            while (CHECK_IPAGE(next_page)) {
                if (bank != next_page.inode_t.bank) {
                    bank = next_page.inode_t.bank;
                    ret = func_004299C4(pfs, &tmp_inode, PFS_READ, bank);
                    if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
                        return ret;
                    }
                }

                if ((cc = func_004287D8(pfs, next_page, &cache) - cl) != 0) {
                    break;
                }

                cl = 1;
                next_page = tmp_inode.inode_page[next_page.inode_t.page];
            }

            if (cc != 0 || next_page.ipage != PFS_EOF) {
                tmp_dir.company_code = 0;
                tmp_dir.game_code = 0;
                tmp_dir.start_page.ipage = 0;
                tmp_dir.status = DIR_STATUS_EMPTY;
                tmp_dir.data_sum = 0;

                SET_ACTIVEBANK_TO_ZERO();
                ERRCK(func_00299A40(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, 0));
                fixed++;
            }
        } else {
            if (tmp_dir.company_code != 0 || tmp_dir.game_code != 0) {
                tmp_dir.company_code = 0;
                tmp_dir.game_code = 0;
                tmp_dir.start_page.ipage = 0;
                tmp_dir.status = DIR_STATUS_EMPTY;
                tmp_dir.data_sum = 0;

                SET_ACTIVEBANK_TO_ZERO();
                ERRCK(func_00299A40(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir, 0));
                fixed++;
            }
        }
    }

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(func_00299680(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&tmp_dir));

        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0 &&
            tmp_dir.start_page.ipage >= (u16)pfs->inode_start_page) {
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        } else {
            file_next_node[j].ipage = 0;
        }
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        ret = func_004299C4(pfs, &tmp_inode, PFS_READ, bank);

        if (ret != 0 && ret != PFS_ERR_INCONSISTENT) {
            return ret;
        }

        offset = (bank > 0) ? 1 : pfs->inode_start_page;

        for (j = 0; j < offset; j++) {
            checked_inode.inode_page[j].ipage = tmp_inode.inode_page[j].ipage;
        }

        for (; j < 128; j++) {
            checked_inode.inode_page[j].ipage = PFS_PAGE_NOT_USED;
        }

        for (j = 0; j < pfs->dir_size; j++) {
            while (file_next_node[j].inode_t.bank == bank &&
                   file_next_node[j].ipage >= (u16)pfs->inode_start_page) {
                u8 pp = file_next_node[j].inode_t.page;
                file_next_node[j] = checked_inode.inode_page[pp] = tmp_inode.inode_page[pp];
            }
        }
        ERRCK(func_004299C4(pfs, &checked_inode, PFS_WRITE, bank));
    }

    if (fixed) {
        pfs->status |= PFS_CORRUPTED;
    } else {
        pfs->status &= ~PFS_CORRUPTED;
    }

    return 0;
}
