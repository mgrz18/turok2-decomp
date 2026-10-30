#include "common.h"

/* Controller / rumble slot, 0x224 bytes (see func_004301C8). */
typedef struct Ctl {
    /* 0x000 */ u8 pad0[0xCC];
    /* 0x0CC */ s32 rumbleState;
    /* 0x0D0 */ u8 padD0[0x224 - 0xD0];
} Ctl;

typedef struct Layer {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ s32 flagsA;
    /* 0x84 */ u8 pad84[0xA8 - 0x84];
    /* 0xA8 */ s32 flagsB;
} Layer;

typedef struct Screen {
    /* 0x0 */ s16 state;
    /* 0x2 */ u8 pad2[0xC - 2];
    /* 0xC */ Layer *layer;
} Screen;

extern Ctl D_800F5EC0[4];
extern s32 D_80130910[4];
extern char D_800F5A50[];
extern u8 D_8011AAF1; /* multiplayer */

void func_0026EDA8(void *);
void func_00285304(void);
void *func_00275F7C(void);
void func_00275DC0(void *);

void func_004531B4(Screen *screen) {
    s32 i;

    screen->state = 2;
    for (i = 0; i < 4; i++) {
        D_80130910[i] = D_800F5EC0[i].rumbleState;
    }
    func_0026EDA8(D_800F5A50);
    func_00285304();
    if (D_8011AAF1) {
        screen->layer->flagsA &= ~0x800000;
        screen->layer->flagsB &= ~0x800000;
    } else {
        screen->layer->flagsA |= 0x800000;
        screen->layer->flagsB |= 0x800000;
    }
    func_00275DC0(func_00275F7C());
}
