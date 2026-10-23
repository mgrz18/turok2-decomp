#include "common.h"

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad04[0x1C];
    /* 0x20 */ f32 rate;
} Func002156FCDef;

typedef struct {
    /* 0x00 */ u8 pad00[0x1C];
    /* 0x1C */ s32 value;
} Func002156FCLink;

typedef struct {
    /* 0x000 */ u8 pad000[0x10];
    /* 0x010 */ Func002156FCLink *link;
    /* 0x014 */ Func002156FCDef *def;
    /* 0x018 */ u8 pad018[0x14C];
    /* 0x164 */ f32 scale;
    /* 0x168 */ u8 pad168[4];
    /* 0x16C */ u16 flags;
    /* 0x16E */ u8 pad16E[2];
    /* 0x170 */ f32 timer;
    /* 0x174 */ f32 speed;
    /* 0x178 */ u8 pad178[0x18];
    /* 0x190 */ s32 linkValue;
    /* 0x194 */ u8 pad194;
    /* 0x195 */ u8 lastFrame;
} Func002156FCObj;

extern s32 D_800B6D00;
extern u8 D_800B6D1B; /* frame counter */
extern f32 D_800B6D28; /* frame time scale */
extern s32 D_8011ACA4;

/* Once-per-frame update. */
void func_002156FC(Func002156FCObj *obj) {
    Func002156FCDef *def;
    f32 timer;

    if (obj->lastFrame == D_800B6D1B || D_8011ACA4 != 0) {
        return;
    }
    def = obj->def;
    if (def->type == 12) {
        obj->speed = D_800B6D28 * def->rate;
    } else if (obj->flags & 0x10) {
        timer = obj->timer + D_800B6D28 * (1.0f / 15.0f);
        obj->timer = timer;
        if (timer < 0.75f) {
            obj->scale = timer * (0.35f / 0.75f);
        } else {
            obj->flags &= ~0x10;
            obj->scale = 0.35f;
        }
    }
    if (obj->link != NULL) {
        obj->linkValue = obj->link->value;
    } else {
        obj->linkValue = D_800B6D00;
    }
    obj->lastFrame = D_800B6D1B;
}
