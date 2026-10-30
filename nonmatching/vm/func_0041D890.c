/* NON-MATCHING: 28 of 102 words differ: the ROM reloads D_800F70A8 after each store into the
 * save header and keeps the save pointer in $a0; the copies and allocation match. */
#include "common.h"

typedef struct {
    /* 0x000 */ s32 data[0x4E8 / 4];
} PlayerBlob;

typedef struct {
    /* 0x000 */ s32 data[0x184 / 4];
} SetupBlob;

typedef struct {
    /* 0x00 */ f32 f[4];
} MatchHead;

typedef struct {
    /* 0x000 */ s32 size;
    /* 0x004 */ s32 extra;
    /* 0x008 */ f32 match[4];
    /* 0x018 */ PlayerBlob player;
    /* 0x500 */ SetupBlob setup;
    /* 0x684 */ char text[4];
} SaveBuffer;

typedef struct {
    /* 0x000 */ u8 pad0[0x520];
    /* 0x520 */ PlayerBlob blob;
} Player;

extern s32 D_800F70A8;
extern s32 *D_800C2010;        /* buffer handle */
extern SaveBuffer *D_800C2018;
extern SetupBlob D_8011AAD8;
extern MatchHead D_8011ACB0;
extern char D_800C13F0[];
extern char D_800EBAC0[];
extern void func_002053A8(s32);
extern void func_002052D8(s32, void *);
extern s32 *func_00204EDC(s32, s32, s32, void *);
extern void func_00266C5C(void *dst, s32 value, s32 size);
extern void func_0029E460(char *dst, char *src, s32 n);

/* Snapshots the player and the multiplayer setup into a freshly allocated buffer. */
void func_0041D890(Player *p) {
    s32 size = D_800F70A8 + 0x688;
    SaveBuffer *save;
    s32 allocSize;
    s32 *handle;
    SaveBuffer *buf;

    if (D_800C2010 != NULL) {
        func_002053A8(0);
        func_002052D8(0, D_800C2010);
    }
    allocSize = D_800F70A8 + 0x688;
    handle = func_00204EDC(0, allocSize, 0x33, D_800C13F0);
    buf = (SaveBuffer *) *handle;
    D_800C2010 = handle;
    D_800C2018 = buf;
    func_00266C5C(D_800C2018, 0, allocSize);
    D_800C2018->player = p->blob;
    D_800C2018->setup = D_8011AAD8;
    save = D_800C2018;
    save->match[0] = D_8011ACB0.f[0];
    save->match[1] = D_8011ACB0.f[1];
    save->match[2] = D_8011ACB0.f[2];
    save->match[3] = D_8011ACB0.f[3];
    /* Header words written through a plain pointer (GCC then reloads D_800F70A8). */
    ((s32 *) save)[0] = size;
    ((s32 *) save)[1] = D_800F70A8;
    func_0029E460(save->text, D_800EBAC0, D_800F70A8);
}
