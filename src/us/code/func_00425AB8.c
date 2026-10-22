/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC.CPP, func_804041F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
extern s32 func_00285878(s32 arg0);
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
typedef struct OSPfs OSPfs;
struct OSPfs {
    int status;
    struct OSMesgQueue *queue;
    int channel;
    u8 id[32];
    u8 label[32];
    int version;
    int dir_size;
    int inode_table;
    int minode_table;
    int dir_table;
    int inode_start_page;
    u8 banks;
    u8 activebank;
};
struct OSPfs;
struct OSMesgQueue;
struct OSMesgQueue;
extern int D_80130C60[];
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
extern u8 D_800F677B[];
extern void func_002858A4();
extern void func_002847E0(void);
struct PakRecord;
struct PakRecord {
    u8 present;
    char pad[7];
    char pak[0x68];
};
struct PakRecord;
typedef struct PakRecord PakRecord;
extern u8 D_800F6750;
extern PakRecord D_80130AA0[];
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
extern s32 func_004258E8(s32);
extern void func_002847E0(void);
extern void func_002858A4(void);
s32 func_004258E8(s32 ch);
void func_00425AB8(void) {
    extern u8 D_800F6798[];
    s32 channel;
    s32 present;
    PakRecord *record;
    func_00285878(1);
    func_002847E0();
    D_800F6750 = 2;
    for (channel = 0; channel < 4; channel++) {
        record = &D_80130AA0[channel];
        if (D_800F677B[channel * 4] != 0) {
            present = 0;
            record->present = 0;
        } else {
            record->present = osMotorInit(D_800F6798, record->pak, channel) == 0;
            present = record->present;
        }
        if (present) D_80130C60[channel] = 2;
        else if (func_004258E8(channel) != 0) D_80130C60[channel] = 3;
        else D_80130C60[channel] = 1;
    }
    func_002858A4();
}
