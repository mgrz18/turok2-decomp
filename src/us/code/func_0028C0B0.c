#include "common.h"

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

typedef struct PlayerEntry {
    /* 0x0000 */ s32 unk0;
    /* 0x0004 */ f32 x, y, z;
    /* 0x0010 */ u8 pad10[0x1320 - 0x10];
    /* 0x1320 */ struct PlayerEntry *next;
} PlayerEntry;

/* "\n": the string literal of the functions before this one in the same file (the ROM merges it), declared so this one does not emit its own copy. */
extern u8 D_800AA0F0[];
extern s32 D_800C2030;              /* screen width */
extern s32 D_800C2034;              /* screen height */
extern u16 *D_80130A88;             /* frame buffer */
extern u16 *D_8011F180;             /* crash screen frame buffer */
extern u16 D_8011F184;              /* text colour */
extern u16 D_800B7498;              /* background colour */
extern char D_8011F188[];           /* build string */
extern char D_8010FED4[];
extern s32 D_80110030;              /* warping */
extern s32 D_80110048;              /* warp id */
extern PlayerEntry *D_80119890;     /* players */
extern char *D_800B25E4;            /* failed assertion: file */
extern s32 D_800F1FF0;              /* failed assertion: line */
extern char *D_800F1FF4;            /* failed assertion: expression */
extern u8 D_800B749C[];             /* cause descriptions */
extern u8 D_800C7C00[];             /* thread stacks */
extern u8 D_800C7C80[];
extern u8 D_800C5400[];
extern u8 D_800CFC80[];
extern volatile s32 D_800B75EC;  /* set when the crash screen may be left */

extern u64 func_0029B170(void);                 /* osGetTime */
extern void func_00424508(s32);
extern void func_0029AC90(u16 *frameBuffer);    /* osViSwapBuffer */
extern void func_0028B430(u8 c);                /* draws one character */
extern void func_0028CF38(char *s);             /* draws a string */
extern void func_0028B614(u32 value, char *name, u8 *descriptions);
extern void func_0028B7C0(char *name1, u32 value1, char *name2, u32 value2);
extern void func_0028BDFC(u8 *stack, s32 size, ThreadContext *context, u8 id);
extern void func_0029E3B0(void);

static __inline__ void clearScreen(void) {
    u16 *p = D_8011F180;
    s32 i;

    for (i = 0; i < D_800C2030 * D_800C2034; i++) {
        *p++ = D_800B7498;
    }
}

static __inline__ void printString(u8 *s) {
    while (*s != 0) {
        func_0028B430(*s++);
    }
}

static __inline__ void printInt(s32 n) {
    char buf[16];
    char *p;
    s32 v;

    if (n < 0) {
        func_0028B430('-');
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
    func_0028CF38(p);
}

/* The crash screen: waits, then shows the build, the players, and the fault or failed assertion with registers and stack. */
void func_0028C0B0(Thread *thread) {
    ThreadContext *context = &thread->context;
    PlayerEntry *player;
    s32 i;
    u64 start;

    start = func_0029B170();
    do {
    } while ((f32)(((func_0029B170() - start) * 64) / 3) * 0.000001f < 4.0f);
    func_00424508(1);
    func_0029AC90(D_80130A88);
    D_8011F184 = 0xA529;
    D_800B7498 = 0x2955;
    D_8011F180 = D_80130A88;
    clearScreen();
    clearScreen();
    D_8011F184 = 0xF7A9;
    printString((u8 *)"TUROK2 BUILD ");
    printString((u8 *)D_8011F188);
    printString(D_800AA0F0);
    printString((u8 *)D_8010FED4);
    printString((u8 *)" ");
    if (D_80110030 != 0) {
        printString((u8 *)"WARP ID   ");
        printInt(D_80110048);
    } else {
        printString((u8 *)"NO WARP ID");
    }
    printString(D_800AA0F0);
    for (player = D_80119890, i = 0; player != NULL; player = player->next, i++) {
        printString((u8 *)"PLAYER");
        printInt(i);
        printString((u8 *)" XYZ ");
        printString((u8 *)" ");
        printInt(player->x * 0.09765625f);
        printString((u8 *)" ");
        printInt(player->y * 0.09765625f);
        printString((u8 *)" ");
        printInt(player->z * 0.09765625f);
        printString(D_800AA0F0);
    }
    printString(D_800AA0F0);
    if (D_800B25E4 != NULL) {
        printString((u8 *)"ASSERTION FAILED!! in thread ");
        printInt(thread->id);
        printString((u8 *)"\nFile: ");
        printString((u8 *)D_800B25E4);
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
        func_0028B614(context->cause, "cause", D_800B749C);
    }
    printString(D_800AA0F0);
    func_0028B7C0("pc", context->pc, "sp", context->sp);
    func_0028B7C0("ra", context->ra, "badvaddr", context->badvaddr);
    func_0028B7C0("a0", context->a0, "a1", context->a1);
    func_0028B7C0("a2", context->a2, "a3", context->a3);
    func_0028B7C0("v0", context->v0, "v1", context->v1);
    func_0028B7C0("sr", context->sr, "fpcsr", context->fpcsr);
    switch (thread->id) {
    case 0:
        func_0028BDFC(D_800C7C00, 0x80, context, thread->id);
        break;
    case 8:
        func_0028BDFC(D_800C5400, 0x2800, context, thread->id);
        break;
    case 7:
        func_0028BDFC(D_800CFC80, 0x2000, context, thread->id);
        break;
    case 1:
        func_0028BDFC(D_800C7C80, 0x8000, context, thread->id);
        break;
    }
    func_0029E3B0();
    while (D_800B75EC == 0) {
    }
}
