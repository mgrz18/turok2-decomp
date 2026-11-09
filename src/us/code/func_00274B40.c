/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/8025D470.CPP, func_8025D450_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "globals/audio.h"
#include "protos/iset.h"
extern void alCSPSetBank(s32 arg0, s32 arg1);
extern void alCSPSetSeq(s32 arg0, s32 arg1);
extern void alCSPSetVol(s32 arg0, s16 arg1);
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct Track;
typedef struct Track Track;
struct Resource;
typedef struct Resource Resource;
struct ContextRecAt2BB8;
typedef struct ContextRecAt2BB8 ContextRecAt2BB8;
struct Resource;
struct Resource {
    s32 unk0;
    u16 unk4;
};
struct ContextRecAt2BB8;
struct ContextRecAt2BB8 {
    char pad[0x2B50];
    s32 unk2B50;
    char p54[12];
    s32 unk2B60;
    s32 unk2B64;
    char p68[0x3C];
    f32 unk2BA4;
    char pa8[16];
    s32 unk2BB8;
};
struct ContextRecAt2BB8;
struct IntPair;
struct Track;
struct Track {
    struct ContextRecAt2BB8 *unk0;
    s32 unk4;
    struct IntPair *unk8;
    void *unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
};
extern void alCSPPlay(s32);
extern int alCSPGetState(void *object);
struct Track;
extern void alCSPSetChlFXMix(s32 arg0, s32 arg1, s8 arg2);
extern s32 BinarySearch(s32,s32,s32);
extern f32 HalfToFloat(u16);
extern void alCSeqNew(s32, void *);
static __inline__ f32 fade_rate(Track *track,f32 time,f32 rate) {track->unk30=time;return (f32)track->unk20/(time*rate);}
void SndSeq_Update(Track *arg0) {
    f32 temp_f0;
    f32 temp_f20;
    f32 var_f20;
    f32 var_f2;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_s0;
    s32 var_v0;
    void *temp_s0_2;
    ContextRecAt2BB8 *temp_v0_2;
    struct Resource *temp_v0_4;
    ContextRecAt2BB8 *temp_v0_5;
    void *var_v0_2;
    temp_v1 = arg0->unk18;
    switch (temp_v1) {
    case 0x20:
        if (arg0->unk1C & 4) {
            arg0->unk1C = 0;
            sSndSeqCurrentId = -1;
            if (arg0->unk28 >= 0) {
                arg0->unk18 = 0x40;
                return;
            }
            arg0->unk18 = 0x10;
            return;
        }
        return;
    case 0x100:
        if (arg0->unk1C & 8) {
            arg0->unk18 = 0x80;
            arg0->unk1C = 0;
            return;
        }
        temp_v0 = sSndSeqLoadRetries + 1;
        sSndSeqLoadRetries = temp_v0;
        if (temp_v0 >= 0xA) {
            sSndSeqCurrentId = -1;
            arg0->unk18 = 0x40;
            return;
        }
        break;
    case 0x40:
        temp_a2 = arg0->unk28;
        var_v0 = 0x10;
        if (temp_a2 != -1) {
            temp_v0_2 = arg0->unk0;
            temp_v0_3 = BinarySearch(temp_v0_2->unk2B60, temp_v0_2->unk2B64, temp_a2);
            temp_s0 = temp_v0_3;
            if (temp_s0 != -1) {
                temp_s0 *= 2;
                temp_s0_2 = CISet__GetBlock(arg0->unk0->unk2B50, temp_s0 | 1);
                temp_v0_4 = CISet__GetBlock(arg0->unk0->unk2B50, temp_s0);
                arg0->unk24 = (s32) temp_v0_4->unk0;
                temp_f0 = HalfToFloat(temp_v0_4->unk4);
                arg0->unk30 = temp_f0;
                if (temp_f0 <= 0.0f) {
                    arg0->unk30 = 0.1f;
                }
                var_v0_2 = temp_s0_2;
            } else {
                var_v0_2 = ((void *)0);
            }
            arg0->unkC = var_v0_2;
            if (var_v0_2 == ((void *)0)) {
                arg0->unk18 = 0x100;
                sSndSeqLoadRetries = 0;
                arg0->unk1C = 0;
                return;
            }
            arg0->unk18 = 0x80;
            arg0->unk4 = 1;
        case 0x80:
            var_s0 = 0;
            alCSeqNew(arg0->unk10, arg0->unkC);
            alCSPSetSeq(arg0->unk14, arg0->unk10);
            alCSPSetBank(arg0->unk14, arg0->unk8->field_4);
            do {
                alCSPSetChlFXMix(arg0->unk14, var_s0 & 0xFF, 0);
                var_s0 += 1;
            } while (var_s0 < 0x14);
            temp_f20 = (f32) arg0->unk24 * (arg0->unk0->unk2BA4 * 0.01f);
            alCSPSetVol(arg0->unk14, (s16) (s32) (temp_f20 * 32767.0f));
            arg0->unk2C = temp_f20;
            alCSPPlay(arg0->unk14);
            arg0->unk1C = 0;
            arg0->unk18 = 0x10;
            sSndSeqCurrentId = arg0->unk28;
            return;
        }
        goto block_44;
    case 0x10:
    default:
        if ((sSndSeqCurrentId > 0) && (alCSPGetState((void *)arg0->unk14) == 0)) {
            sSndSeqCurrentId = 0;
            arg0->unk28 = 0;
        } else {
            if (arg0->unk38 != 0) {
                var_f20 = (f32) arg0->unk24;
                var_f20 *= arg0->unk0->unk2BA4 * 0.01f;
                var_f20 *= arg0->unk3C;
                goto block_37;
            }
            temp_v0_5 = arg0->unk0;
            var_f20 = (f32) arg0->unk24 * (temp_v0_5->unk2BA4 * 0.01f);
            if (temp_v0_5->unk2BB8 != 0) {
                var_f20 *= 0.7f;
            }
            if (var_f20 != arg0->unk2C) {
block_37:
                alCSPSetVol(arg0->unk14, (s16) (s32) (var_f20 * 32767.0f));
                arg0->unk2C = var_f20;
            }
        }
        if (arg0->unk28 != sSndSeqCurrentId) {
            if (sSndSeqCurrentId > 0) {
                var_f2 = arg0->unk30;
                arg0->unk1C = 2;
                arg0->unk18 = 0x20;
                arg0->unk20 = (s32) (arg0->unk2C * 32767.0f);
                if (!(var_f2 >= 0.1f)) {
                    var_f2 = 0.1f;
                }
                arg0->unk34 = fade_rate(arg0,var_f2,6.0e1f);
                return;
            }
            var_v0 = 0x40;
            arg0->unk1C = 0;
block_44:
            arg0->unk18 = var_v0;
        }
        break;
    }
}
