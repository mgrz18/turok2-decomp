/* t2audio_bank.c: the sound bank and the sound data file for the host (docs/port/AUDIO.md §2).
 *
 * On the N64, CAudioWorld__Initialize maps ROM 0x1E0000..0x20F194 (the "ctl region") at the VM
 * address 0x480000 with VM_Alloc and uses it in place: its header is
 *     s32 count; { ALBankFile *file; s32 tblOffset; }[count]
 * and every pointer inside it was pre-relocated by the build tools to that VM address (the
 * game never calls alBnkfNew). On the host the structs are larger (8-byte pointers), so the
 * bank graph is rebuilt here: every ALBankFile, ALBank, ALInstrument, ALSound, ALEnvelope,
 * ALKeyMap, ALWaveTable, ALADPCMloop and ALADPCMBook becomes a host object in host byte order
 * (the game's own struct definitions, include/structs/libaudio.h), shared objects stay shared.
 *
 * ALSound order: CAudioWorld__Initialize sorts each instrument's soundArray by pointer value.
 * On the N64 that is ROM order; here every bank's sounds are allocated in one array in ROM
 * address order, so the host pointers sort the same way.
 *
 * ALWaveTable.base stays what the ROM holds (an offset from gAudioRomBase, the bank's table
 * offset already added); the host __amDMA (t2audio_mgr.c) resolves it.
 *
 * The sound data file (ROM 0xBE1814, CAudioWorld__LoadSfxDataFile; layout in
 * tools/assets/swap_spec.json "sound data file" and tools/assets/audio.py parse_tree) is used
 * in place by the game: T2Audio_SwapSoundData swaps its ISet/USet headers and typed records
 * to host order once, when VM_Alloc makes the resident copy. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "structs/libaudio.h"
#include "t2port.h"
#include "t2audio.h"

#define CTL_ROM 0x1E0000u
#define CTL_SIZE 0x2F194u
#define CTL_VADDR 0x480000u
#define SOUND_DATA_ROM 0xBE1814u

static const u8 *sRom;
static size_t sRomSize;

static u32 be32(u32 a) {
    const u8 *p = sRom + a;
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}
static u16 be16(u32 a) {
    const u8 *p = sRom + a;
    return (u16)((p[0] << 8) | p[1]);
}

/* ctl VM pointer -> ROM offset (0 if outside the region) */
static u32 vrom(u32 v) {
    if (v < CTL_VADDR || v >= CTL_VADDR + CTL_SIZE)
        return 0;
    return v - CTL_VADDR + CTL_ROM;
}

/* ---- memo: ROM address -> host object (shared objects stay shared) ---- */

typedef struct {
    u32 rom;
    void *host;
} Memo;
static Memo *sMemo;
static int sMemoCount, sMemoCap;

static void *memo_get(u32 rom) {
    int i;
    for (i = 0; i < sMemoCount; i++)
        if (sMemo[i].rom == rom)
            return sMemo[i].host;
    return NULL;
}
static void memo_put(u32 rom, void *host) {
    if (sMemoCount == sMemoCap) {
        sMemoCap = sMemoCap ? sMemoCap * 2 : 1024;
        sMemo = realloc(sMemo, sizeof(Memo) * (size_t)sMemoCap);
    }
    sMemo[sMemoCount].rom = rom;
    sMemo[sMemoCount].host = host;
    sMemoCount++;
}

static void *halloc(size_t n) {
    return t2_lowmem_alloc(n); /* zeroed; below 2 GB in low memory mode */
}

static T2AudioCtl sCtl;
static int sCtlDone;

static ALEnvelope *conv_envelope(u32 a) {
    ALEnvelope *e = memo_get(a);
    if (e)
        return e;
    e = halloc(sizeof *e);
    e->attackTime = (s32)be32(a);
    e->decayTime = (s32)be32(a + 4);
    e->releaseTime = (s32)be32(a + 8);
    e->attackVolume = sRom[a + 12];
    e->decayVolume = sRom[a + 13];
    memo_put(a, e);
    return e;
}

static ALKeyMap *conv_keymap(u32 a) {
    ALKeyMap *k = memo_get(a);
    if (k)
        return k;
    k = halloc(sizeof *k);
    k->velocityMin = sRom[a];
    k->velocityMax = sRom[a + 1];
    k->keyMin = sRom[a + 2];
    k->keyMax = sRom[a + 3];
    k->keyBase = sRom[a + 4];
    k->detune = (s8)sRom[a + 5];
    memo_put(a, k);
    return k;
}

static ALWaveTable *conv_wave(u32 a) {
    ALWaveTable *w = memo_get(a);
    u32 lp, bk;
    int i;
    if (w)
        return w;
    w = halloc(sizeof *w);
    w->base = (u8 *)(uptr)be32(a);
    w->len = (s32)be32(a + 4);
    w->type = sRom[a + 8];
    w->flags = sRom[a + 9];
    lp = vrom(be32(a + 12));
    bk = vrom(be32(a + 16));
    if (w->type == AL_ADPCM_WAVE) {
        if (lp) {
            ALADPCMloop *l = memo_get(lp);
            if (!l) {
                l = halloc(sizeof *l);
                l->start = be32(lp);
                l->end = be32(lp + 4);
                l->count = be32(lp + 8);
                for (i = 0; i < 16; i++)
                    l->state[i] = (s16)be16(lp + 12 + 2 * i);
                memo_put(lp, l);
            }
            w->waveInfo.adpcmWave.loop = l;
        }
        if (bk) {
            ALADPCMBook *b = memo_get(bk);
            if (!b) {
                s32 order = (s32)be32(bk), npred = (s32)be32(bk + 4);
                s32 n = order * npred * 8;
                if (n < 0 || n > 4096)
                    n = 0;
                b = halloc(sizeof *b + sizeof(s16) * (size_t)(n + 8));
                b->order = order;
                b->npredictors = npred;
                for (i = 0; i < n; i++)
                    b->book[i] = (s16)be16(bk + 8 + 2 * i);
                memo_put(bk, b);
            }
            w->waveInfo.adpcmWave.book = b;
        }
    } else if (w->type == AL_RAW16_WAVE && lp) {
        ALRawLoop *l = memo_get(lp);
        if (!l) {
            l = halloc(sizeof *l);
            l->start = be32(lp);
            l->end = be32(lp + 4);
            l->count = be32(lp + 8);
            memo_put(lp, l);
        }
        w->waveInfo.rawWave.loop = l;
    }
    sCtl.waves++;
    memo_put(a, w);
    return w;
}

/* the sounds of one bank file, collected first so they can be laid out in ROM order */
static u32 *sSoundRoms;
static int sSoundCount, sSoundCap;

static void collect_sound(u32 a) {
    int i;
    for (i = 0; i < sSoundCount; i++)
        if (sSoundRoms[i] == a)
            return;
    if (sSoundCount == sSoundCap) {
        sSoundCap = sSoundCap ? sSoundCap * 2 : 256;
        sSoundRoms = realloc(sSoundRoms, sizeof(u32) * (size_t)sSoundCap);
    }
    sSoundRoms[sSoundCount++] = a;
}
static void collect_inst(u32 a) {
    s16 n = (s16)be16(a + 14);
    int i;
    for (i = 0; i < n; i++) {
        u32 s = vrom(be32(a + 16 + 4 * i));
        if (s)
            collect_sound(s);
    }
}
static int cmp_u32(const void *x, const void *y) {
    u32 a = *(const u32 *)x, b = *(const u32 *)y;
    return a < b ? -1 : a > b;
}

static void make_sounds(void) {
    ALSound *arr;
    int i;
    qsort(sSoundRoms, (size_t)sSoundCount, sizeof(u32), cmp_u32);
    arr = halloc(sizeof(ALSound) * (size_t)(sSoundCount ? sSoundCount : 1));
    for (i = 0; i < sSoundCount; i++) {
        u32 a = sSoundRoms[i];
        ALSound *s = &arr[i];
        u32 env = vrom(be32(a)), km = vrom(be32(a + 4)), wt = vrom(be32(a + 8));
        s->envelope = env ? conv_envelope(env) : NULL;
        s->keyMap = km ? conv_keymap(km) : NULL;
        s->wavetable = wt ? conv_wave(wt) : NULL;
        s->samplePan = sRom[a + 12];
        s->sampleVolume = sRom[a + 13];
        s->flags = sRom[a + 14];
        memo_put(a, s);
    }
    sCtl.sounds += sSoundCount;
    sSoundCount = 0;
}

static ALInstrument *conv_inst(u32 a) {
    ALInstrument *t = memo_get(a);
    s16 n;
    int i;
    if (t)
        return t;
    n = (s16)be16(a + 14);
    t = halloc(sizeof *t + sizeof(ALSound *) * (size_t)(n > 0 ? n : 1));
    t->volume = sRom[a];
    t->pan = sRom[a + 1];
    t->priority = sRom[a + 2];
    t->flags = sRom[a + 3];
    t->tremType = sRom[a + 4];
    t->tremRate = sRom[a + 5];
    t->tremDepth = sRom[a + 6];
    t->tremDelay = sRom[a + 7];
    t->vibType = sRom[a + 8];
    t->vibRate = sRom[a + 9];
    t->vibDepth = sRom[a + 10];
    t->vibDelay = sRom[a + 11];
    t->bendRange = (s16)be16(a + 12);
    t->soundCount = n;
    for (i = 0; i < n; i++)
        t->soundArray[i] = memo_get(vrom(be32(a + 16 + 4 * i)));
    sCtl.instruments++;
    memo_put(a, t);
    return t;
}

static ALBankFile *conv_bankfile(u32 a) {
    s16 nb;
    ALBankFile *f;
    int b, i;
    if ((s16)be16(a) != 0x4231) {
        fprintf(stderr, "t2audio: no B1 bank file at ROM %X\n", a);
        return NULL;
    }
    nb = (s16)be16(a + 2);
    /* pass 1: the sounds, in ROM order */
    for (b = 0; b < nb; b++) {
        u32 ba = vrom(be32(a + 4 + 4 * b));
        s16 ic;
        u32 perc;
        if (!ba)
            continue;
        ic = (s16)be16(ba);
        perc = vrom(be32(ba + 8));
        if (perc)
            collect_inst(perc);
        for (i = 0; i < ic; i++) {
            u32 ia = vrom(be32(ba + 12 + 4 * i));
            if (ia)
                collect_inst(ia);
        }
    }
    make_sounds();
    /* pass 2: banks and instruments */
    f = halloc(sizeof *f + sizeof(ALBank *) * (size_t)(nb > 0 ? nb : 1));
    f->revision = 0x4231;
    f->bankCount = nb;
    for (b = 0; b < nb; b++) {
        u32 ba = vrom(be32(a + 4 + 4 * b));
        s16 ic;
        u32 perc;
        ALBank *k;
        if (!ba)
            continue;
        ic = (s16)be16(ba);
        k = halloc(sizeof *k + sizeof(ALInstrument *) * (size_t)(ic > 0 ? ic : 1));
        k->instCount = ic;
        k->flags = sRom[ba + 2];
        k->pad = sRom[ba + 3];
        k->sampleRate = (s32)be32(ba + 4);
        perc = vrom(be32(ba + 8));
        k->percussion = perc ? conv_inst(perc) : NULL;
        for (i = 0; i < ic; i++) {
            u32 ia = vrom(be32(ba + 12 + 4 * i));
            k->instArray[i] = ia ? conv_inst(ia) : NULL;
        }
        f->bankArray[b] = k;
        sCtl.banks++;
    }
    return f;
}

const T2AudioCtl *T2Audio_LoadCtl(void) {
    u32 n;
    if (sCtlDone)
        return &sCtl;
    sCtlDone = 1;
    sRom = t2_rom(&sRomSize);
    if (sRom == NULL || sRomSize < CTL_ROM + CTL_SIZE) {
        fprintf(stderr, "t2audio: no ROM image: no sound bank\n");
        return &sCtl;
    }
    n = be32(CTL_ROM);
    if (n >= 1) {
        sCtl.musicBank = conv_bankfile(vrom(be32(CTL_ROM + 4)));
        sCtl.musicTbl = be32(CTL_ROM + 8);
    }
    if (n >= 2) {
        sCtl.sfxBank = conv_bankfile(vrom(be32(CTL_ROM + 12)));
        sCtl.sfxTbl = be32(CTL_ROM + 16);
    }
    fprintf(stderr, "t2audio: sound bank converted: %d bank files, %d banks, %d instruments, %d sounds, %d wave tables\n",
            (int)n, sCtl.banks, sCtl.instruments, sCtl.sounds, sCtl.waves);
    free(sMemo);
    sMemo = NULL;
    sMemoCount = sMemoCap = 0;
    free(sSoundRoms);
    sSoundRoms = NULL;
    sSoundCap = 0;
    return &sCtl;
}

/* ---- the sound data file ---- */

static long sSwapped;
static u8 *sBase;
static size_t sSize;

static int in_range(size_t a, size_t n) {
    return a <= sSize && n <= sSize - a;
}
static u32 rd32(size_t a) { /* big-endian, before swapping */
    const u8 *p = sBase + a;
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}
static void sw32(size_t a, size_t n) {
    size_t i;
    if (!in_range(a, n * 4))
        return;
    for (i = 0; i < n; i++) {
        u8 *p = sBase + a + 4 * i, t;
        t = p[0], p[0] = p[3], p[3] = t;
        t = p[1], p[1] = p[2], p[2] = t;
    }
    sSwapped += (long)n;
}
static void sw16(size_t a, size_t n) {
    size_t i;
    if (!in_range(a, n * 2))
        return;
    for (i = 0; i < n; i++) {
        u8 *p = sBase + a + 2 * i, t;
        t = p[0], p[0] = p[1], p[1] = t;
    }
    sSwapped += (long)n;
}

/* ISet (romfs.parse_iset, CISet__GetBlock): u32 count; u32 offs[count + 1] (relative to the
 * set; the last is its size). Fills offs (host order) and swaps the header. Returns count or -1. */
static int iset(size_t a, size_t size, u32 *offs, int max) {
    u32 n, i;
    if (!in_range(a, 8) || size < 8)
        return -1;
    n = rd32(a);
    if (n > (u32)max || !in_range(a, 8 + 4 * (size_t)n))
        return -1;
    for (i = 0; i <= n; i++) {
        offs[i] = rd32(a + 4 + 4 * i);
        if (offs[i] > size || (i && offs[i] < offs[i - 1]))
            return -1;
    }
    sw32(a, 2 + n);
    return (int)n;
}

/* USet: u32 elemSize; u32 count; elem[count]. Swaps the header; returns count, sets *es. */
static int uset(size_t a, size_t size, u32 *es) {
    u32 e, n;
    if (size < 8 || !in_range(a, 8))
        return -1;
    e = rd32(a);
    n = rd32(a + 4);
    if (e != 0 && (size_t)n > (size - 8) / e)
        return -1;
    sw32(a, 2);
    *es = e;
    return (int)n;
}

/* a USet of s32 */
static int s32set(size_t a, size_t size) {
    u32 es;
    int n = uset(a, size, &es);
    if (n < 0 || (es != 4 && n))
        return -1;
    sw32(a + 8, (size_t)n);
    return 0;
}

/* SFX layer: s16 group, s16 sample, s8 priority, s8 chance, u16 flags, s16 unk8, s16 unkA,
 * s16 unkC, s8 pitch, s8 unkF, s16 unk10, s16 randomRange0, s16 randomRange1, pad[2] */
static void sfx_layer(size_t a) {
    sw16(a, 2);
    sw16(a + 6, 4);
    sw16(a + 0x10, 3);
}

/* SndEnv (structs/sound.h): u16 level, u16 unk2, u16 target, s16 frames, s16 delay, u16 unkA */
static int env_set(size_t a, size_t size) {
    u32 es;
    int n = uset(a, size, &es), i;
    if (n < 0)
        return -1;
    for (i = 0; i < n; i++)
        sw16(a + 8 + es * (size_t)i, es / 2 < 6 ? es / 2 : 6);
    return 0;
}

/* SongData: 0x44 -> ALCMidiHdr (s32[16] trackOffset, s32 division) + track bytes;
 * else the adec stream: u32 sampleRate, frameCount, bitstreamBytes + u16 words */
static void song_data(size_t a, size_t size) {
    if (size >= 0x44 && rd32(a) == 0x44) {
        sw32(a, 17);
    } else if (size >= 12) {
        sw32(a, 3);
        sw16(a + 12, (size - 12) / 2);
    }
}

#define MAXSET 4096
long T2Audio_SwapSoundData(uint8_t *p, size_t size) {
    static u32 top[MAXSET + 1], sfx[MAXSET + 1], fx[MAXSET + 1], parts[MAXSET + 1], songs[MAXSET + 1],
        blocks[MAXSET + 1];
    int nt, ns, nf, i, j;
    sBase = p;
    sSize = size;
    sSwapped = 0;
    nt = iset(0, size, top, MAXSET);
    if (nt < 2)
        return -1;
    /* top[0]: the sfx set */
    ns = iset(top[0], top[1] - top[0], sfx, MAXSET);
    if (ns >= 6) {
        size_t sa = top[0];
        s32set(sa + sfx[0], sfx[1] - sfx[0]); /* effect ranges */
        s32set(sa + sfx[1], sfx[2] - sfx[1]); /* weights */
        /* the effects: each an ISet { cue bytes, USet of layers } */
        nf = iset(sa + sfx[2], sfx[3] - sfx[2], fx, MAXSET);
        for (i = 0; i < nf; i++) {
            size_t ea = sa + sfx[2] + fx[i];
            int np = iset(ea, fx[i + 1] - fx[i], parts, MAXSET);
            if (np >= 2) {
                u32 es;
                size_t la = ea + parts[1];
                int nl = uset(la, parts[2] - parts[1], &es);
                for (j = 0; j < nl; j++)
                    sfx_layer(la + 8 + es * (size_t)j);
            }
        }
        env_set(sa + sfx[3], sfx[4] - sfx[3]);
        env_set(sa + sfx[4], sfx[5] - sfx[4]);
        env_set(sa + sfx[5], sfx[6] - sfx[5]);
    }
    /* top[1]: the songs: ISet { s32 USet of keys, ISet of (header, data) pairs } */
    if (iset(top[1], top[2] - top[1], songs, MAXSET) >= 2) {
        size_t sa = top[1];
        int nb;
        s32set(sa + songs[0], songs[1] - songs[0]);
        nb = iset(sa + songs[1], songs[2] - songs[1], blocks, MAXSET);
        for (i = 0; i + 1 < nb; i += 2) {
            size_t ha = sa + songs[1] + blocks[i];
            size_t da = sa + songs[1] + blocks[i + 1];
            if (blocks[i + 1] - blocks[i] >= 8) {
                sw32(ha, 1); /* s32 volume */
                sw16(ha + 4, 2); /* u16 scale, u16 h6 */
            }
            song_data(da, blocks[i + 2] - blocks[i + 1]);
        }
    }
    return sSwapped;
}

void T2Audio_FixRegion(uint32_t romAddr, uint8_t *p, size_t size) {
    if (romAddr == SOUND_DATA_ROM && T2Audio_Enabled()) {
        long n = T2Audio_SwapSoundData(p, size);
        fprintf(stderr, "t2audio: sound data file (ROM %X, %zu bytes): %ld words swapped to host order\n", romAddr,
                size, n);
    }
}
