/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/802591C0.CPP, func_80258AE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;

typedef struct CVector3 {
    f32 x, y, z;
} CVector3;

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

/* Match globals; in Turok 2 the match state lives in the same block. */
typedef struct MenuMatchGlobals {
    char pad0[0x13D8];
    s32 state;          /* 0x13D8 (D_8011AC88) */
} MenuMatchGlobals;

extern int D_800B2680;
extern MenuMatchGlobals D_801198B0;
extern s32 func_00270C80(void *, s32, CVector3, s32, s32);
extern void *func_00281444(MenuMatchGlobals *arg0, CVector3 *arg1);

void func_00271694(ContextRecAt2BC0 *arg0) {
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
        result = func_00281444(&D_801198B0, &zero);
        vec = &((SharedRecAt564V1 *)(result))->position;
        if ((arg0->frame & 3) == 0) {
            func_00270C80(arg0, arg0->track, *vec, 0, -1);
        }
    }
}
