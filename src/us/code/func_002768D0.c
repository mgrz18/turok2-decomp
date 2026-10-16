/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_8.c, func_8025CC90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct SongHeader;
struct SongHeader {
    u32 tempo;
    u16 volume;
};
struct SongHeader;
typedef struct SongHeader SongHeader;
struct Bank;
struct Bank {
    char pad0[0x2B50];
    void *songs;
};
struct Bank;
struct PlayerRecAt24;
struct PlayerRecAt24 {
    struct Bank *bank;
    char pad4[4];
    u32 state;
    s32 loop;
    s32 song;
    s32 speed;
    char pad18[3];
    u8 priority;
    char pad1C[2];
    s16 id;
    f32 tempoScale;
    f32 volume;
};
struct QueueRec;
typedef struct QueueRec QueueRec;
struct QueueRec;
struct QueueRec {
    void *sequence;
    s32 head;
    u8 kind;
    s32 tail;
};
struct Bank;
typedef struct Bank Bank;
struct PlayerRecAt24;
typedef struct PlayerRecAt24 PlayerRecAt24;
struct func_80203E78_S1;
struct func_80203E78_S1 {
    char pad0[0x4];
    s32 unk4;
};
struct func_80203E78_S1;
typedef struct func_80203E78_S1 func_80203E78_S1;
extern void func_002A12DC();
extern QueueRec D_800F55F4[];
extern void func_002A12CC(void);
extern s32 func_002A1170(void *, s32, s32 *);
extern s32 func_00293580(char *);
extern void func_00293DE0(char *, f32);
extern void func_00293E70(char *, s32);
extern void func_00293D90(char *, s32);
extern void func_00293E30(char *, s32, s32);
extern void func_00293D40(char *, s32);
extern void func_00293CE0(char *);
void func_002768D0(PlayerRecAt24 *player) {
    extern void func_00293E60(char *, s32);
    extern void *func_002017D4(void *, s32);
    extern char *func_002718F4(Bank *);
    extern void func_00271514(Bank *);
    extern void func_002714B0(Bank *);
    extern func_80203E78_S1 *D_800B2AA8;
    s32 rate;
    char *channel;
    s32 index;
    SongHeader *header;
    void *sequence;
    func_002714B0(player->bank);
    channel = func_002718F4(player->bank);
    switch (player->state) {
        case 0:
            break;
        case 3:
            func_002A12CC();
            break;
        case 2:
            func_00293E60(channel, player->id);
            if (func_00293580(channel) == 0) {
                func_002A12DC();
                if (player->loop >= 0) {
                    player->state = 1;
                } else {
                    player->state = 0;
                }
            }
            break;
        case 1:
            ((s32 *)D_800F55F4)[0] = 0;
            ((u8 *)D_800F55F4)[4] = 2;
            ((s32 *)D_800F55F4)[2] = 0;
            index = player->song * 2;
            header = func_002017D4(player->bank->songs, index);
            player->tempoScale = header->tempo * 0.01f;
            player->volume = header->volume;
            sequence = func_002017D4(player->bank->songs, index | 1);
            ((void **)D_800F55F4)[-1] = sequence;
            D_800B2AA8->unk4 = func_002A1170(sequence, 0x5622, &rate);
            player->state = 3;
            func_00293E60(channel, player->id);
            func_00293DE0(channel, rate * 4.5351473e-05f);
            func_00293E70(channel, (s16)(player->speed * player->tempoScale));
            func_00293D90(channel, player->priority);
            func_00293E30(channel, player->id, 0x7F);
            func_00293D40(channel, (u8)(u32)player->volume);
            func_00293CE0(channel);
            player->loop = -1;
            break;
    }
    func_00271514(player->bank);
}
