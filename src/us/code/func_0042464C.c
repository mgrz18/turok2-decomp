/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_22.CPP, func_8040C484_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct NodeRecAt294;
typedef struct NodeRecAt294 NodeRecAt294;
struct NodeRecAt294;
struct NodeRecAt294 {
    int unused;
    struct NodeRecAt294 *next;
    char pad[0x280];
    float width;
    float height;
    int x;
    int y;
};
extern int D_80000300;
extern int D_800C2030;
extern int D_800C2034;
extern int D_80000300,D_800AB894;
extern unsigned short D_800C21AA[],D_801309D0;
extern NodeRecAt294 D_801198F0;
extern int D_800C2030,D_800C2034;
extern void func_00424008(unsigned,int,int,int,int,int);
void func_0042464C(unsigned width,int a1,int a2,int a3,int a4,int a5) {
 int flags=0,region=0; NodeRecAt294 *node; float h;
 switch(D_80000300) {case 0:region=1;break;case 1:break;case 2:region=2;break;}
 if(width>320) flags|=4;
 if(D_800AB894) flags|=2;
 if(width>320) flags|=1;
 D_801309D0=*(unsigned short *)((char *)D_800C21AA+flags*4+region*32);
 func_00424008(width,a1,a2,a3,a4,a5);
 node=&D_801198F0;
 if(node) {node->width=D_800C2030;h=D_800C2034;node->x=0;node->y=0;node->height=h;}
 node=*(NodeRecAt294 **)((char *)node-32);
 while(node) {node->x=0;node->y=0;node->width=D_800C2030;node->height=D_800C2034;node=node->next;}
}
