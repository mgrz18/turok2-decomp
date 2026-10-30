/* NON-MATCHING: 24 of 102 words differ (checked at the real start): D_800F70A8 is read twice directly at the top but through a CSE'd address (and reloaded after the extraSize store) at the end; as an array its address goes to $s0 for the whole function. */
/* WRONG CUT: the real function starts at 0x004567B0 (size 0x198 = 408): the word
 * at 0x004567AC is junk between objects; callers jal 0x0041D7B0 (this module is
 * linked 0x39000 lower). Kept under splat's name for the checker. */
#include "common.h"

/* Build the save block: the player's profile, the multiplayer settings and the
 * player's options are copied into a freshly allocated buffer. */

typedef struct {
    s32 w[0x4E8 / 4];
} ProfileBlob;

typedef struct {
    s32 w[0x184 / 4];
} SettingsBlob;

typedef struct {
    s32 size;              /* 0x000 */
    s32 extraSize;         /* 0x004 */
    f32 options[4];        /* 0x008 */
    ProfileBlob profile;   /* 0x018 */
    SettingsBlob settings; /* 0x500 */
    u8 extra[1];           /* 0x684 */
} SaveBlock;

typedef struct {
    u8 pad0[0x520];
    ProfileBlob profile; /* 0x520 */
} Player;

extern s32 D_800F70A8[]; /* [0]: size of the extra data */
extern void **D_800C2010; /* handle of the save block */
extern SaveBlock *D_800C2018;
extern SettingsBlob D_8011AAD8;
extern f32 D_8011ACB0[4];
extern u8 D_800EBAC0[];
extern char D_800C13F0[];

void func_002053A8(s32 heap);
void func_002052D8(s32 heap, void **handle);
void **func_00204EDC(s32 heap, s32 size, s32 tag, char *name);
void func_00266C5C(void *dst, s32 value, s32 size);
void func_0029E460(void *dst, void *src, s32 size);

void func_004567AC(Player *player) {
    s32 size = D_800F70A8[0] + 0x688;
    s32 allocSize;
    SaveBlock *data;

    if (D_800C2010 != NULL) {
        func_002053A8(0);
        func_002052D8(0, D_800C2010);
    }
    allocSize = D_800F70A8[0] + 0x688;
    D_800C2010 = func_00204EDC(0, allocSize, 0x33, D_800C13F0);
    D_800C2018 = *D_800C2010;
    func_00266C5C(D_800C2018, 0, allocSize);
    D_800C2018->profile = player->profile;
    D_800C2018->settings = D_8011AAD8;
    data = D_800C2018;
    data->options[0] = D_8011ACB0[0];
    data->options[1] = D_8011ACB0[1];
    data->options[2] = D_8011ACB0[2];
    data->options[3] = D_8011ACB0[3];
    data->size = size;
    data->extraSize = D_800F70A8[0];
    func_0029E460(data->extra, D_800EBAC0, D_800F70A8[0]);
}
