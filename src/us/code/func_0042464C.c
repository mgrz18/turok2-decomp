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
extern int osTvType;
extern int gScreenWidth;
extern int gScreenHeight;
extern int osTvType,gViAntiAlias;
extern unsigned short D_800C21AA[],sViModeIndex;
extern NodeRecAt294 gEngineApp_m_Game_m_CameraPool_m_MainCamera; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern int gScreenWidth,gScreenHeight;
extern void Video_ResizeViMode(unsigned,int,int,int,int,int);
void Video_SetMode(unsigned width,int a1,int a2,int a3,int a4,int a5) {
 int flags=0,region=0; NodeRecAt294 *node; float h;
 switch(osTvType) {case 0:region=1;break;case 1:break;case 2:region=2;break;}
 if(width>320) flags|=4;
 if(gViAntiAlias) flags|=2;
 if(width>320) flags|=1;
 sViModeIndex=*(unsigned short *)((char *)D_800C21AA+flags*4+region*32);
 Video_ResizeViMode(width,a1,a2,a3,a4,a5);
 node=&gEngineApp_m_Game_m_CameraPool_m_MainCamera;
 if(node) {node->width=gScreenWidth;h=gScreenHeight;node->x=0;node->y=0;node->height=h;}
 node=*(NodeRecAt294 **)((char *)node-32);
 while(node) {node->x=0;node->y=0;node->width=gScreenWidth;node->height=gScreenHeight;node=node->next;}
}
