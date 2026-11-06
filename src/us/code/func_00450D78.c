/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E1C0.c, func_0043E1C0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
/* gEngineApp_m_Settings_match (GameMatch in structs/game.h; unk24 is teams). Kept as a local
 * array view: through it the compiler bases the store and the argument on
 * gEngineApp_m_Settings_match + 0x24, as the ROM does; through the struct it bases them on
 * gEngineApp_m_Settings_match. */
typedef struct S10Game {
               u8 pad0[0x14];
               f32 unk14;
               s32 unk18;
               s32 unk1C;
               s32 unk20;
               s32 unk24;
               s32 unk28;
               s32 unk2C[5];
               s32 unk40[5];
               s32 unk54;
               u8 pad58[0x6C - 0x58];
               s32 unk6C;
               u8 pad70[0x78 - 0x70];
               s32 unk78;
               u8 pad7C[4];
               s32 unk80;
} S10Game;
extern S10Game gEngineApp_m_Settings_match[];
extern f32 sFinalScoreTimer;
extern f32 gFrameIncrement;
extern void CPlayerPool__EndMatch(void *p);
s32 FinalScoreMenu_Poll_2(void) {
    sFinalScoreTimer += gFrameIncrement;
    if (sFinalScoreTimer >= 120.0f) {
        gEngineApp_m_Settings_match->unk24 = 0;
        CPlayerPool__EndMatch((u8 *)gEngineApp_m_Settings_match - 0x1440);
        return 1;
    }
    return 0;
}
