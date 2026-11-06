#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/802591C0.CPP, func_80258AE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


typedef struct ContextRecAt2BC0 {
    char pad0[0x104];
    s32 frame;          /* 0x104 */
    char pad108[0x134 - 0x108];
    s32 track;          /* 0x134 */
    char pad138[0x2BB4 - 0x138];
    s32 field2BB4;      /* 0x2BB4 */
} ContextRecAt2BC0;

typedef struct SharedRecAt564V1 {
    char pad0[0x114];
    CVector3 position;  /* 0x114 (0x128 in Rage Wars) */
} SharedRecAt564V1;

/* The camera pool D_801198B0 (CCameraPool, structs/camera.h) seen to the game
 * mode D_8011AAD8.mode (+0x13D8): the ROM reads the mode through this symbol,
 * and through D_8011AAD8 the code no longer matches. */
typedef struct MenuMatchGlobals {
    char pad0[0x13D8];
    s32 state;          /* 0x13D8 (D_8011AC88) */
} MenuMatchGlobals;

extern int D_800B2680;
extern MenuMatchGlobals D_801198B0;
extern s32 CAudioWorld__PlaySound(void *, s32, CVector3, s32, s32);
extern void *CCameraPool__GetNearestCamera(MenuMatchGlobals *arg0, CVector3 *arg1);

void CAudioWorld__UpdateAmbientTrack(ContextRecAt2BC0 *arg0) {
    CVector3 zero;
    void *result;
    CVector3 *vec;
    if (D_800B2680 != 0 &&
        arg0->field2BB4 != 0 &&
        D_801198B0.state == 0xD && /* a call in Rage Wars */
        arg0->track > 0) {
        {
            register f32 value = 0.0f;
            zero.z = value;
            zero.y = value;
            zero.x = value;
        }
        result = CCameraPool__GetNearestCamera(&D_801198B0, &zero);
        vec = &((SharedRecAt564V1 *)(result))->position;
        if ((arg0->frame & 3) == 0) {
            CAudioWorld__PlaySound(arg0, arg0->track, *vec, 0, -1);
        }
    }
}
