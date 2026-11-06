/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80293A04_3.CPP, func_80294158_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
extern s32 gScreenWidth;
extern char *sDemoWarningText[];
extern void Render_DrawTextLines(void *arg0, s32 arg1, char **arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7);
void EngineMode02_DemoWarning_Draw(void *arg0) {
    Render_DrawTextLines(arg0, 1, sDemoWarningText, gScreenWidth / 2, 5, 0xA, 0.75f, 1.0f);
}
