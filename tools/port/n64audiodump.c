/* A mupen64plus audio plugin that writes what the game hands the AI (the N64's DAC input) to a
   file instead of playing it: the N64 audio reference for docs/port/AUDIO.md (round 20, i5).

   Built and loaded by refshots.py --audio-dump OUT.raw (cc -shared -fPIC). Output: raw stereo
   s16 little-endian at the AI rate (VI clock / (dacrate + 1)); refshots writes OUT.raw.txt with
   the sample count at every emulator frame (n64ad_samples()). Never commit dumps. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M64P_PLUGIN_PROTOTYPES 1
#include <mupen64plus/m64p_common.h>
#include <mupen64plus/m64p_plugin.h>
#include <mupen64plus/m64p_types.h>

static AUDIO_INFO ai;
static FILE *out;
static long long nsamples;
static unsigned dacrate;

EXPORT m64p_error CALL PluginStartup(m64p_dynlib_handle core, void *ctx,
                                     void (*cb)(void *, int, const char *)) {
    (void)core; (void)ctx; (void)cb;
    const char *p = getenv("T2_N64_AUDIO_DUMP");
    out = fopen(p ? p : "n64audio.raw", "wb");
    return out ? M64ERR_SUCCESS : M64ERR_FILES;
}

EXPORT m64p_error CALL PluginShutdown(void) {
    if (out)
        fclose(out);
    out = NULL;
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL PluginGetVersion(m64p_plugin_type *t, int *v, int *api, const char **name,
                                        int *caps) {
    if (t) *t = M64PLUGIN_AUDIO;
    if (v) *v = 0x010000;
    if (api) *api = 0x020000;
    if (name) *name = "t2 AI dump";
    if (caps) *caps = 0;
    return M64ERR_SUCCESS;
}

EXPORT int CALL InitiateAudio(AUDIO_INFO info) { ai = info; return 1; }
EXPORT int CALL RomOpen(void) { return 1; }
EXPORT void CALL RomClosed(void) { if (out) fflush(out); }
EXPORT void CALL AiDacrateChanged(int system) { (void)system; dacrate = *ai.AI_DACRATE_REG; }

EXPORT void CALL AiLenChanged(void) {
    unsigned addr = *ai.AI_DRAM_ADDR_REG & 0xFFFFF8, len = *ai.AI_LEN_REG & 0x3FFF8, i;
    if (!out || !len)
        return;
    for (i = 0; i < len; i += 4) {
        /* RDRAM is kept as host-order 32-bit words: L is the high half, R the low */
        unsigned w = *(unsigned *)(ai.RDRAM + ((addr + i) & 0x7FFFFC));
        short s[2] = { (short)(w >> 16), (short)(w & 0xFFFF) };
        fwrite(s, 2, 2, out);
    }
    nsamples += len / 4;
}

EXPORT void CALL ProcessAList(void) {}
EXPORT void CALL SetSpeedFactor(int p) { (void)p; }
EXPORT void CALL VolumeUp(void) {}
EXPORT void CALL VolumeDown(void) {}
EXPORT int CALL VolumeGetLevel(void) { return 100; }
EXPORT void CALL VolumeSetLevel(int l) { (void)l; }
EXPORT void CALL VolumeMute(void) {}
EXPORT const char *CALL VolumeGetString(void) { return "100%"; }

/* for refshots: the samples written so far and the current dacrate */
EXPORT long long n64ad_samples(void) { return nsamples; }
EXPORT unsigned n64ad_dacrate(void) { return dacrate; }
