#ifndef ULTRA_TYPES_H
#define ULTRA_TYPES_H

/* libultra types (PR/os_thread.h, os_message.h, os_time.h, os_cont.h,
 * os_pfs.h and the Controller Pak internals of PRinternal/controller.h), as
 * decompals/ultralib defines them. The SDK code the game links (and the code
 * imported from the Rage Wars decomps) used to carry its own copy of these
 * in every file. */

#include "common.h"

typedef s32 OSPri;
typedef s32 OSId;
typedef void *OSMesg;
typedef u64 OSTime;

typedef union {
    struct {
        f32 f_odd;
        f32 f_even;
    } f;
    f64 d;
} __OSfp;

typedef struct {
    u64 at, v0, v1, a0, a1, a2, a3;
    u64 t0, t1, t2, t3, t4, t5, t6, t7;
    u64 s0, s1, s2, s3, s4, s5, s6, s7;
    u64 t8, t9;
    u64 gp, sp, s8, ra;
    u64 lo, hi;
    u32 sr, pc, cause, badvaddr, rcp;
    u32 fpcsr;
    __OSfp  fp0,  fp2,  fp4,  fp6,  fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext; /* size 0x190 */

typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;

typedef struct OSThread_s {
    /* 0x00 */ struct OSThread_s *next;     /* run/mesg queue link */
    /* 0x04 */ OSPri priority;              /* run/mesg queue priority */
    /* 0x08 */ struct OSThread_s **queue;   /* queue thread is on */
    /* 0x0C */ struct OSThread_s *tlnext;   /* all threads queue link */
    /* 0x10 */ u16 state;                   /* OS_STATE_* */
    /* 0x12 */ u16 flags;                   /* flags for rmon */
    /* 0x14 */ OSId id;                     /* id for debugging */
    /* 0x18 */ int fp;                      /* thread has used fp unit */
    /* 0x1C */ __OSThreadprofile_s *thprof; /* workarea for thread profiler */
    /* 0x20 */ __OSThreadContext context;   /* register/interrupt mask */
} OSThread; /* size 0x1B0 */

typedef struct OSMesgQueue_s {
    /* 0x00 */ OSThread *mtqueue;   /* threads blocked on an empty queue (receive) */
    /* 0x04 */ OSThread *fullqueue; /* threads blocked on a full queue (send) */
    /* 0x08 */ s32 validCount;      /* number of valid messages */
    /* 0x0C */ s32 first;           /* index of the first valid message */
    /* 0x10 */ s32 msgCount;        /* total number of messages */
    /* 0x14 */ OSMesg *msg;         /* message buffer array */
} OSMesgQueue; /* size 0x18 */

typedef struct OSTimer_s {
    /* 0x00 */ struct OSTimer_s *next;
    /* 0x04 */ struct OSTimer_s *prev;
    /* 0x08 */ OSTime interval;
    /* 0x10 */ OSTime value;
    /* 0x18 */ OSMesgQueue *mq;
    /* 0x1C */ OSMesg msg;
} OSTimer; /* size 0x20 */

typedef struct {
    /* 0x0 */ u16 type;
    /* 0x2 */ u8 status;
    /* 0x3 */ u8 errno;
} OSContStatus;

typedef struct {
    /* 0x0 */ u16 button;
    /* 0x2 */ s8 stick_x;
    /* 0x3 */ s8 stick_y;
    /* 0x4 */ u8 errno;
} OSContPad;

typedef struct {
    /* 0x00 */ int status;
    /* 0x04 */ OSMesgQueue *queue;
    /* 0x08 */ int channel;
    /* 0x0C */ u8 id[32];
    /* 0x2C */ u8 label[32];
    /* 0x4C */ int version;
    /* 0x50 */ int dir_size;
    /* 0x54 */ int inode_table;      /* block location */
    /* 0x58 */ int minode_table;     /* mirror of inode_table */
    /* 0x5C */ int dir_table;        /* block location */
    /* 0x60 */ int inode_start_page; /* page # */
    /* 0x64 */ u8 banks;
    /* 0x65 */ u8 activebank;
} OSPfs; /* size 0x68 */

/* What osPfsFileState fills for one note. */
typedef struct {
    /* 0x00 */ u32 file_size;       /* bytes; 0: the note is free */
    /* 0x04 */ u32 game_code;
    /* 0x08 */ u16 company_code;
    /* 0x0A */ char ext_name[4];
    /* 0x0E */ char game_name[16];
} OSPfsState; /* size 0x20 */

/* Controller Pak internals (PRinternal/controller.h). */
typedef union {
    struct {
        /* 0x0 */ u8 bank;
        /* 0x1 */ u8 page;
    } inode_t;
    /* 0x0 */ u16 ipage;
} __OSInodeUnit;

typedef struct {
    /* 0x00 */ u32 game_code;
    /* 0x04 */ u16 company_code;
    /* 0x06 */ __OSInodeUnit start_page;
    /* 0x08 */ u8 status;
    /* 0x09 */ s8 reserved;
    /* 0x0A */ u16 data_sum;
    /* 0x0C */ u8 ext_name[4];
    /* 0x10 */ u8 game_name[16];
} __OSDir; /* size 0x20 */

typedef struct {
    /* 0x0 */ __OSInodeUnit inode_page[128];
} __OSInode; /* size 0x100 */

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
} __OSPackId; /* size 0x20 */

/* pfschecker.c's cache of one bank's inode table. */
typedef struct {
    /* 0x000 */ __OSInode inode;
    /* 0x100 */ u8 bank;
    /* 0x101 */ u8 map[256];
} __OSInodeCache; /* size 0x202 */

/* Not libultra: the engine's recursive lock (Rage Wars' CMutex, built by
 * CMutex__Construct), a one-message queue and the number of holders. Taken
 * with __osDisableInt, ++count and osRecvMesg when count was not 0; released
 * with --count and osSendMesg when holders remain (CMutex__Wait). */
typedef struct CMutex {
    /* 0x00 */ OSMesgQueue queue;
    /* 0x18 */ OSMesg msg;   /* the queue's one-message buffer */
    /* 0x1C */ s32 count;    /* holders, waiting ones included */
} CMutex; /* size 0x20 */

/* The video interface (PR/os_vi.h, viint.h). __osViCurr / __osViNext are
 * D_800B7AC0 / D_800B7AC4 (osViSetEvent, osViSwapBuffer, osViSetYScale... set
 * the next one; __osViSwapContext, __osViSwapContext, loads it). */
typedef struct {
    /* 0x00 */ u32 ctrl;
    /* 0x04 */ u32 width;
    /* 0x08 */ u32 burst;
    /* 0x0C */ u32 vSync;
    /* 0x10 */ u32 hSync;
    /* 0x14 */ u32 leap;
    /* 0x18 */ u32 hStart;
    /* 0x1C */ u32 xScale;
    /* 0x20 */ u32 vCurrent;
} OSViCommonRegs; /* size 0x24 */

typedef struct {
    /* 0x00 */ u32 origin;
    /* 0x04 */ u32 yScale;
    /* 0x08 */ u32 vStart;
    /* 0x0C */ u32 vBurst;
    /* 0x10 */ u32 vIntr;
} OSViFieldRegs; /* size 0x14 */

typedef struct OSViMode {
    /* 0x00 */ u8 type;
    /* 0x04 */ OSViCommonRegs comRegs;
    /* 0x28 */ OSViFieldRegs fldRegs[2];
} OSViMode; /* size 0x50 */

typedef struct {
    /* 0x0 */ f32 factor;
    /* 0x4 */ u16 offset;
    /* 0x8 */ u32 scale;
} __OSViScale; /* size 0xC */

#define VI_STATE_MODE_UPDATED   0x01
#define VI_STATE_XSCALE_UPDATED 0x02
#define VI_STATE_YSCALE_UPDATED 0x04
#define VI_STATE_CTRL_UPDATED   0x08
#define VI_STATE_BUFFER_UPDATED 0x10
#define VI_STATE_BLACK          0x20

typedef struct __OSViContext {
    /* 0x00 */ u16 state;          /* VI_STATE_* */
    /* 0x02 */ u16 retraceCount;
    /* 0x04 */ void *framep;
    /* 0x08 */ OSViMode *modep;
    /* 0x0C */ u32 control;
    /* 0x10 */ OSMesgQueue *msgq;
    /* 0x14 */ OSMesg msg;
    /* 0x18 */ __OSViScale x;
    /* 0x24 */ __OSViScale y;
} __OSViContext; /* size 0x30 */

#endif
