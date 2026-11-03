/* Player code: func_0042EB04, 0x0042EB04-0x0042EB4C (splat had it inside the data D_0042D50C). */
#include "common.h"

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x1288];
    /* 0x1288 */ s32 sound0;
    /* 0x128C */ s32 sound1;
} Player;

extern void *func_00275F7C(void);
extern void func_00275D34(void *pool, s32 sound);

/* Stops the player's two looping sounds. */
void func_0042EB04(Player *p) {
    func_00275D34(func_00275F7C(), p->sound0);
    p->sound0 = 0;
    func_00275D34(func_00275F7C(), p->sound1);
    p->sound1 = 0;
}
