#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct {
    /* 0x000 */ s32 data[0x184 / 4];
} SetupBlob;

typedef struct {
    /* 0x000 */ s32 size;
    /* 0x004 */ s32 extra;
    /* 0x008 */ f32 match[4];
    /* 0x018 */ CPlayerLoadSaveData player;
    /* 0x500 */ SetupBlob setup;
    /* 0x684 */ char text[4];
} SaveBuffer;

typedef struct {
    /* 0x0 */ s32 size;
} SaveSize;

extern SaveSize D_800F70A8;
extern s32 *D_800C2010;        /* buffer handle */
extern SaveBuffer *D_800C2018;
extern char D_800C13F0[];
extern char D_800EBAC0[];
extern void func_002053A8(s32);
extern void func_002052D8(s32, void *);
extern s32 *func_00204EDC(s32, s32, s32, void *);
extern void func_00266C5C(void *dst, s32 value, s32 size);
extern void func_0029E460(char *dst, char *src, s32 n);

static __inline__ s32 SaveFileSize(void) {
    return D_800F70A8.size + 0x688;
}

/* Snapshots the player and the multiplayer setup into a freshly allocated buffer. */
void func_0041D890(CPlayer *p) {
    s32 size = SaveFileSize();
    SaveBuffer *save;
    s32 allocSize;
    s32 *handle;
    SaveBuffer *buf;

    if (D_800C2010 != NULL) {
        func_002053A8(0);
        func_002052D8(0, D_800C2010);
    }
    allocSize = SaveFileSize();
    handle = func_00204EDC(0, allocSize, 0x33, D_800C13F0);
    buf = (SaveBuffer *) *handle;
    D_800C2010 = handle;
    D_800C2018 = buf;
    func_00266C5C(D_800C2018, 0, allocSize);
    D_800C2018->player = p->m_Data;
    D_800C2018->setup = *(SetupBlob *)&D_8011AAD8;
    save = D_800C2018;
    save->match[0] = D_8011ACB0.frames;
    save->match[1] = D_8011ACB0.seconds;
    save->match[2] = D_8011ACB0.minutes;
    save->match[3] = D_8011ACB0.hours;
    /* D_800F70A8 is a struct field here, so it is reloaded after the store into the header. */
    save->size = size;
    save->extra = D_800F70A8.size;
    func_0029E460(save->text, D_800EBAC0, D_800F70A8.size);
}
