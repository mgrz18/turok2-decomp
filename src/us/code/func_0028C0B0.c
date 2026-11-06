#include "common.h"
#include "structs/player.h"

/* libultra __OSThreadContext: 64-bit registers, read here as their low words. */
typedef struct ThreadContext {
    /* 0x000 */ u64 at, v0, v1, a0, a1, a2, a3;
    /* 0x038 */ u64 t0, t1, t2, t3, t4, t5, t6, t7;
    /* 0x078 */ u64 s0, s1, s2, s3, s4, s5, s6, s7;
    /* 0x0B8 */ u64 t8, t9, gp, sp, s8, ra;
    /* 0x0E8 */ u64 lo, hi;
    /* 0x0F8 */ u32 sr, pc, cause, badvaddr, rcp;
    /* 0x10C */ u32 fpcsr;
} ThreadContext;

typedef struct Thread {
    /* 0x00 */ struct Thread *next;
    /* 0x04 */ s32 priority;
    /* 0x08 */ void *queue;
    /* 0x0C */ void *tlnext;
    /* 0x10 */ u16 state;
    /* 0x12 */ u16 flags;
    /* 0x14 */ s32 id;
    /* 0x18 */ s32 fp;
    /* 0x1C */ s32 pad1C;
    /* 0x20 */ ThreadContext context;
} Thread;

/* "\n": the string literal of the functions before this one in the same file (the ROM merges it), declared so this one does not emit its own copy. */
extern u8 D_800AA0F0[];
extern s32 gScreenWidth;              /* screen width */
extern s32 gScreenHeight;              /* screen height */
extern u16 *gCFB;             /* frame buffer */
extern u16 *sCrashFrameBuffer;             /* crash screen frame buffer */
extern u16 sCrashTextColor;              /* text colour */
extern u16 sCrashBgColor;              /* background colour */
extern char gEngineTimestamp[];           /* build string */
extern char D_8010FED4[];
extern s32 gScene_m_bWarpSpawn;              /* warping */
extern s32 gScene_m_LevelID;              /* warp id */
extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;     /* players */
extern char *gAssertFile;            /* failed assertion: file */
extern s32 D_800F1FF0;              /* failed assertion: line */
extern char *D_800F1FF4;            /* failed assertion: expression */
extern u8 gCrashCauseDesc[];             /* cause descriptions */
extern u8 gThreadStackIdle[];             /* thread stacks */
extern u8 gThreadStackMain[];
extern u8 gThreadStackAudio[];
extern u8 gSchedulerStack[];
extern volatile s32 sCrashCanExit;  /* set when the crash screen may be left */

extern u64 osGetTime(void);                 /* osGetTime */
extern void Video_SetResolution(s32);
extern void osViSwapBuffer(u16 *frameBuffer);    /* osViSwapBuffer */
extern void Crash_PutChar(u8 c);                /* draws one character */
extern void Crash_PrintString(char *s);             /* draws a string */
extern void Crash_PrintFlags(u32 value, char *name, u8 *descriptions);
extern void Crash_PrintRegPair(char *name1, u32 value1, char *name2, u32 value2);
extern void Crash_PrintStack(u8 *stack, s32 size, ThreadContext *context, u8 id);
extern void osWritebackDCacheAll(void);

static __inline__ void clearScreen(void) {
    u16 *p = sCrashFrameBuffer;
    s32 i;

    for (i = 0; i < gScreenWidth * gScreenHeight; i++) {
        *p++ = sCrashBgColor;
    }
}

static __inline__ void printString(u8 *s) {
    while (*s != 0) {
        Crash_PutChar(*s++);
    }
}

static __inline__ void printInt(s32 n) {
    char buf[16];
    char *p;
    s32 v;

    if (n < 0) {
        Crash_PutChar('-');
        v = -n;
    } else {
        v = n;
    }
    p = &buf[15];
    *p = 0;
    do {
        *--p = v % 10 + '0';
        v /= 10;
    } while (v != 0);
    Crash_PrintString(p);
}

/* The crash screen: waits, then shows the build, the players, and the fault or failed assertion with registers and stack. */
void Crash_ShowScreen(Thread *thread) {
    ThreadContext *context = &thread->context;
    CPlayer *player;
    s32 i;
    u64 start;

    start = osGetTime();
    do {
    } while ((f32)(((osGetTime() - start) * 64) / 3) * 0.000001f < 4.0f);
    Video_SetResolution(1);
    osViSwapBuffer(gCFB);
    sCrashTextColor = 0xA529;
    sCrashBgColor = 0x2955;
    sCrashFrameBuffer = gCFB;
    clearScreen();
    clearScreen();
    sCrashTextColor = 0xF7A9;
    printString((u8 *)"TUROK2 BUILD ");
    printString((u8 *)gEngineTimestamp);
    printString(D_800AA0F0);
    printString((u8 *)D_8010FED4);
    printString((u8 *)" ");
    if (gScene_m_bWarpSpawn != 0) {
        printString((u8 *)"WARP ID   ");
        printInt(gScene_m_LevelID);
    } else {
        printString((u8 *)"NO WARP ID");
    }
    printString(D_800AA0F0);
    for (player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead, i = 0; player != NULL; player = player->m_pNext, i++) {
        printString((u8 *)"PLAYER");
        printInt(i);
        printString((u8 *)" XYZ ");
        printString((u8 *)" ");
        printInt(player->m_CharacterActor.header.m_vPos.x * 0.09765625f);
        printString((u8 *)" ");
        printInt(player->m_CharacterActor.header.m_vPos.y * 0.09765625f);
        printString((u8 *)" ");
        printInt(player->m_CharacterActor.header.m_vPos.z * 0.09765625f);
        printString(D_800AA0F0);
    }
    printString(D_800AA0F0);
    if (gAssertFile != NULL) {
        printString((u8 *)"ASSERTION FAILED!! in thread ");
        printInt(thread->id);
        printString((u8 *)"\nFile: ");
        printString((u8 *)gAssertFile);
        printString((u8 *)"  Line: ");
        printInt(D_800F1FF0);
        printString(D_800AA0F0);
        if (D_800F1FF4 != NULL) {
            printString((u8 *)"(");
            printString((u8 *)D_800F1FF4);
            printString((u8 *)") = 0");
        }
        printString(D_800AA0F0);
    } else {
        printString((u8 *)"Fault in thread ");
        printInt(thread->id);
        printString(D_800AA0F0);
        Crash_PrintFlags(context->cause, "cause", gCrashCauseDesc);
    }
    printString(D_800AA0F0);
    Crash_PrintRegPair("pc", context->pc, "sp", context->sp);
    Crash_PrintRegPair("ra", context->ra, "badvaddr", context->badvaddr);
    Crash_PrintRegPair("a0", context->a0, "a1", context->a1);
    Crash_PrintRegPair("a2", context->a2, "a3", context->a3);
    Crash_PrintRegPair("v0", context->v0, "v1", context->v1);
    Crash_PrintRegPair("sr", context->sr, "fpcsr", context->fpcsr);
    switch (thread->id) {
    case 0:
        Crash_PrintStack(gThreadStackIdle, 0x80, context, thread->id);
        break;
    case 8:
        Crash_PrintStack(gThreadStackAudio, 0x2800, context, thread->id);
        break;
    case 7:
        Crash_PrintStack(gSchedulerStack, 0x2000, context, thread->id);
        break;
    case 1:
        Crash_PrintStack(gThreadStackMain, 0x8000, context, thread->id);
        break;
    }
    osWritebackDCacheAll();
    while (sCrashCanExit == 0) {
    }
}
