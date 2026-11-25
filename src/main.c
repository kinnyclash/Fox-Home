/* FOX HOME v0.11.18 - TI-84 Plus CE (C, CE C/C++ toolchain)
 * Port of Kinny's Python game. Created with love by Kinny.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include "sprites.h"

#define OY 15   /* game area is 320x210, centered on the 240px screen */
#define TY 8    /* text offset inside a row */
#define CW 8    /* font width (monospace) */
#define SAVE_NAME "FOXSAVE"
#define BG_NAME "FOXBG"

/* ---------- colors ---------- */
#define a5 0x37262AUL
#define G 0xFFEFD3UL
#define aA 0x4B3132UL
#define aR 0xDE434CUL
#define aH 0x428DE0UL
#define aw 0xEF9131UL
#define a8 0xF4C78BUL
#define a9 0xF6DEBEUL
#define aT 0xCDB6A3UL
#define bE 0x14692AUL
#define bO 0x4EA0CDUL
#define bC 0x271C1BUL
#define bo 0x31272BUL
#define bq 0xB57A4EUL
#define br 0x9B695EUL
#define bJ 0xAE7B6AUL
#define bm 0xDCB57EUL
#define bI 0xE2EFF6UL
#define bt 0x69AB5BUL
#define bM 0x437D61UL
static const uint32_t bNr[3]={15387562UL,14076636UL,15653550UL};
static const uint32_t bG[4]={16430479UL,7382743UL,11882868UL,1647435UL};
static const uint8_t bA[4][3]={{106,95,84},{100,100,100},{100,72,65},{43,50,69}};
static const uint32_t aI[24]={15696933UL,16773054UL,16758078UL,14805232UL,16777215UL,12701921UL,6910078UL,14607854UL,10398136UL,15710775UL,16773566UL,16766569UL,8868661UL,15717038UL,11630926UL,11623215UL,15655112UL,7826278UL,15059876UL,16775399UL,16244663UL,3751500UL,11450310UL,6318205UL};
static const uint32_t cP[9]={0xD93642UL,0xF58A32UL,0xF5D84EUL,0x48A65DUL,0x4087DDUL,0x8756C5UL,0xF19CC5UL,0xF5F3EDUL,0x30313DUL};

/* ---------- text ---------- */
static const char *const bk[8]={"Red","Arctic","Silver","Golden","Chocolate","Cross","Cream","Black"};
static const char *const bD[4]={"Classic","Socks","Patches","Dark tail"};
static const char *const a1[3]={"Male","Female","Nonbinary"};
static const char *const a7[4]={"Morning","Day","Evening","Night"};
static const char *const a7l[4]={"morning","day","evening","night"};
static const char *const aS[6]={"Living Room","Bedroom","Kitchen","Outside","Snow","Pool"};
static const char *const at[7]={"Berries","Apple","Fish","Chicken Wing","Sweet Berries","Pumpkin Pie","Rabbit"};
static const uint8_t aO[7]={8,15,30,50,12,24,40};
static const uint8_t a0[7]={15,30,55,100,22,45,80};
static const char *const aZ[5]={"Minnow","Trout","Salmon","Koi","Golden fish"};
static const uint8_t aM[5]={55,30,12,2,1};
static const uint16_t aN[5]={10,25,60,200,1000};
static const char *const bB[20]={"Yip!","Yip yip!",":3","Mfnfhgghfhfh","Mrrrp!","Awoo~","Eeeep!","Mlem :3","Nhnnhnh!","Yrrrrp!","Mrrf mrrf","Yip! :3","mrrrrrrp","mrow:3","yip yip!","ring ding ding ding ding ding ding ding ding","meowwwllll","rrrrrruf!","awa!!",">w<"};
typedef struct{const char *txt,*act;char ty;uint8_t amt,item;}ev_t;
static const ev_t EV[6][4]={
 {{"Coins under the couch!","Search",'c',25,0},{"Your fox has the zoomies!","Play",'j',2,0},{"A lost toy appears!","Play",'j',1,0},{"Your fox brings a gift!","Open",'r',1,0}},
 {{"Your fox had a sweet dream!","Cuddle",'j',2,0},{"A cozy blanket awaits!","Snuggle",'h',15,0},{"Coins under the pillow!","Collect",'c',15,0},{"Your fox wants a lullaby!","Sing",'j',1,0}},
 {{"An apple rolls off the table!","Pick up",'f',1,1},{"A pie is cooling!","Collect",'f',1,5},{"Your fox smells a snack!","Share",'u',20,0},{"Change in the cookie jar!","Collect",'c',20,0}},
 {{"A butterfly flutters past!","Catch",'b',1,0},{"A rabbit left berries!","Collect",'f',1,0},{"A storm scares your fox!","Comfort",'j',2,0},{"A coin glints in the grass!","Collect",'c',30,0}},
 {{"A snowflake lands on your fox!","Play",'j',1,0},{"A snowball fight begins!","Play",'j',2,0},{"A frozen treasure appears!","Dig",'c',40,0},{"Your fox is shivering!","Warm up",'h',12,0}},
 {{"A fish jumps from the pool!","Catch",'f',1,2},{"Your fox wants to splash!","Play",'j',2,0},{"A floating coin appears!","Collect",'c',20,0},{"Your fox needs a towel!","Dry off",'h',10,0}}};
static const ev_t STAR={"A shooting star appears!","Make wish",'c',50,0};
static const ev_t TRAV={"A traveler brings a gift!","Accept",'r',1,0};
static const char *const bl[3][7]={
 {"ENTER Pet / event","1-9 Switch fox","0 Pets / adopt","ALPHA / LEFT Food shop","MODE / RIGHT Food bag","VARS / 2ND Rooms","DOWN Clothes"},
 {"MATH Minigames","DEL Dismiss / release pet","CLEAR Save and quit","STAT Guide","UP/DOWN Select","ENTER Confirm","CLEAR Back in menus"},
 {"Clothes: LEFT/RIGHT category","UP/DOWN item","ENTER Buy/equip/remove","Simon: number keys","Sumo: ENTER taps","Fishing: ENTER hook",""}};
static const char *const gName[3]={"Simon Says","Sumo","Fishing"};
static const char *const gDesc[3]={"Remember 10 digits","50 taps in 10 seconds","Hook fish for rewards"};
#define VERSION "FOX HOME v0.11.18"
static const char aQ[]="Not enough money!";
static const char a3[]="ENTER Select / CLEAR Back";
static const char a4[]="ENTER Start / CLEAR Back";
static const char a2[]="DEL Dismiss";
static const char *const cN[9]={"Red","Orange","Yellow","Green","Blue","Purple","Pink","White","Black"};
static const char *const cH[6]={"Winter hat","Crown","Fedora","Flower","Bow","Wizard hat"};
static const char *const cA[5]={"Gay rainbow bandana","Lesbian bandana","Bi bandana","Trans bandana","Nonbinary bandana"};
static const char *const cT[3]={"HATS","SHIRTS","ACCESSORIES"};
static const uint8_t cL[3]={6,18,15},cS[3]={0,3,8},cB[3]={7,31,15};

/* ---------- state ---------- */
typedef struct{char nm[13];uint8_t gen,hp,joy;uint32_t lv;uint8_t food,hung,wake,rest,room,col,pat;}pet_t;
typedef struct{char magic[4];uint8_t ver,ae,nF;uint32_t H;uint16_t T;uint32_t V[7],ap[5],cM[3];uint16_t cO[9];pet_t F[9];}save_t;
static pet_t F[9];static uint8_t nF,ae;
static uint32_t H,V[7],ap[5],cM[3],ax;static uint16_t cO[9];static int T;
#define PT (F[ae])
enum{S_HOME,S_CONTROLS,S_PETS,S_GAMES,S_ROOMS,S_SHOP,S_BAG,S_CLOTHES};
static uint8_t C=S_HOME,B,cC,ak,av;static bool I,aW;
static const ev_t *L;static const char *ad;
static char K[40],E[48],aj[40];
static const uint8_t *bgdata;

/* ---------- helpers ---------- */
static int rnd(int a,int b){return a+(int)((unsigned)rand()%(unsigned)(b-a+1));}
static int imin(int a,int b){return a<b?a:b;}
static int imax(int a,int b){return a>b?a:b;}
static uint32_t ms(void){return (uint32_t)(((unsigned long long)clock()*1000ULL)/CLOCKS_PER_SEC);}

/* dynamic palette: entries 1..127 hold colors used by the game, 128..255 the current background */
static uint32_t pc[128];static uint8_t pn=1;static bool pfull,dirty;
static uint8_t ci(uint32_t c){
 uint8_t i,best=1;long bd=0x7FFFFFFFL;
 dirty=true;c&=0xFFFFFFUL;
 for(i=1;i<pn;i++)if(pc[i]==c)return i;
 if(pn<128){pc[pn]=c;gfx_palette[pn]=gfx_RGBTo1555((uint8_t)(c>>16),(uint8_t)(c>>8),(uint8_t)c);return pn++;}
 pfull=true;
 for(i=1;i<128;i++){long dr=(long)(pc[i]>>16)-(long)(c>>16),dg=(long)(pc[i]>>8&255)-(long)(c>>8&255),db=(long)(pc[i]&255)-(long)(c&255);long d=dr*dr+dg*dg+db*db;if(d<bd){bd=d;best=i;}}
 return best;
}
static void U(int d,int e,int c,int b,uint32_t a){if(c>0&&b>0){gfx_SetColor(ci(a));gfx_FillRectangle(d-1,e-1+OY,c+1,b+1);}}
static void an(int d,int e,int c,int a,bool b){U(d,e,c,a,b?a8:a9);}
static void ao(int a){U(0,0,320,210,G);U(0,0,320,a,aA);}
static void A(int f,int g,const char *s,int e,int d,uint32_t c){
 char buf[48];int b=e/CW,n=(int)strlen(s);
 if(!b)return;
 if(b>47)b=47;
 if(n>b){if(b>1){memcpy(buf,s,b-1);buf[b-1]='.';}else buf[0]=s[0];buf[b]=0;n=b;}else strcpy(buf,s);
 if(d)f+=(e-n*CW)/(3-d);
 gfx_SetTextFGColor(ci(c));gfx_PrintStringXY(buf,f,g+TY+OY);
}
#define At(f,g,s,e,d) A(f,g,s,e,d,a5)
static void aU(char *o,uint32_t c,int b){
 static const uint32_t dv[3]={1000000000UL,1000000UL,1000UL};static const char sf[3]={'B','M','K'};
 char e[16];int i;
 sprintf(o,"%lu",(unsigned long)c);
 if((int)strlen(o)<=b)return;
 for(i=0;i<3;i++)if(c>=dv[i]){sprintf(e,"%lu%c",(unsigned long)(c/dv[i]),sf[i]);if((int)strlen(e)<=b){strcpy(o,e);return;}}
 o[b-1]='+';o[b]=0;
}
static uint32_t bi(uint32_t a){const uint8_t *m=bA[T/300];
 uint32_t r=(a>>16)*m[0]/100,g=(a>>8&255)*m[1]/100,b=(a&255)*m[2]/100;
 if(r>255)r=255;if(g>255)g=255;if(b>255)b=255;return r<<16|g<<8|b;}
static void M(int f,int b,int e,int c,uint32_t a,bool d){int g;if(d)a=bi(a);g=46+(b+c-62)*110/108;b=46+(b-62)*110/108;gfx_SetColor(ci(a));gfx_FillRectangle(f-1,b-1+OY,e+1,g-b+1);}

/* ---------- sprites ---------- */
static void cRr(int x,int y,uint32_t c,const uint8_t *g,uint8_t n,bool t){
 uint8_t j;gfx_SetColor(ci(t?bi(c):c));
 for(j=0;j<n;j++,g+=4)gfx_FillRectangle(x+g[0]-49,y+g[1]-49+OY,g[2]-47,g[3]-47);
}
static uint32_t cFc(uint32_t c,uint8_t o){
 uint32_t b,f,r,g,bb;
 if(!o||!PT.col)return c;
 b=aI[PT.col*3+o-1];f=o==1?0xF79D5BUL:0xFFFEFFUL;
 r=(c>>16)*(b>>16)/(f>>16);g=(c>>8&255)*(b>>8&255)/(f>>8&255);bb=(c&255)*(b&255)/(f&255);
 if(r>255)r=255;if(g>255)g=255;if(bb>255)bb=255;return r<<16|g<<8|bb;
}
static void cSp(int x,int y,bool t,uint8_t shape,const uint32_t *col){
 const shape_t *s=&SHAPES[shape];uint8_t i;
 for(i=0;i<s->nl;i++)cRr(x+28,y+28,col[i],s->r[i],s->n[i],t);
}
static void cR(int k,int l,int a,bool t,uint32_t c,const char *s){
 gfx_SetColor(ci(t?bi(c):c));
 for(;*s;s+=4)gfx_FillRectangle(k+(s[0]-48)*a-1,l+(s[1]-48)*a-1+OY,(s[2]-48)*a+1,(s[3]-48)*a+1);
}
#define HX 34
#define HY 9
#define GY 16
static const uint32_t hatA[6]={0xCB394DUL,0xF4C547UL,0x353343UL,0xF19CC5UL,0xE9519AUL,0x7654B4UL};
static const uint32_t hatB[6]={G,0xFFE98AUL,0xA88C74UL,0xF5D84EUL,0xA62D65UL,0xE8C95AUL};
static const char *const hatSA[6]={"0.62","0/610-112-114-11","1.420/61","4/215-115/126011","0/222/113/22","2,112-221.42"};
static const char *const hatSB[6]={"0/612-21","00114111","1/41","5/11","2/11","0/61"};
static void bd(int k,int l,bool r,bool t){
 const fox_t *e=&FOX[r];uint8_t i,h,s,f;uint16_t o;
 k-=r?4:3;l-=r?19:11;
 for(i=0;i<e->nl;i++)cRr(k,l,cFc(e->l[i].col,e->l[i].role),e->l[i].r,e->l[i].n,t);
 if(PT.pat){uint32_t pc2=PT.pat==1?aI[PT.col*3+2]:PT.pat==2?aI[PT.col*3+1]:bo;cRr(k,l,pc2,e->pat[PT.pat-1],e->pn[PT.pat-1],t);}
 if(r)return;
 o=cO[ae];h=o&7;s=o>>3&31;f=o>>8&15;
 if(s){
  if(s<10)cSp(k-8,l-16,t,SHIRTS[s-1].shape,SHIRTS[s-1].col);
  else{uint32_t c=cP[(s-1)%9],cc[2];cc[0]=((c>>2)&0x3F3F3FUL)*3;cc[1]=c;cSp(k-8,l-16,t,SHIRTS[1].shape,cc);}
 }
 if(f){
  if(f<6)cSp(k-16,l-17,t,ACCS[f-1].shape,ACCS[f-1].col);
  else if(f<15)cSp(k-11,l-21,t,ACCS[f-1].shape,ACCS[f-1].col);
  else cR(k+HX,l+GY,4,t,0x242630UL,"003221113032");
 }
 if(h){cR(k+HX,l+HY,4,t,hatA[h-1],hatSA[h-1]);cR(k+HX,l+HY,4,t,hatB[h-1],hatSB[h-1]);}
}
static bool bb(void){return T/300==3&&PT.wake==0;}
static bool drawBG(int h){
 const uint8_t *p;int i;
 if(!bgdata)return false;
 p=bgdata+4+h*(384+2+160*55);
 for(i=0;i<128;i++,p+=3)gfx_palette[128+i]=gfx_RGBTo1555(p[0],p[1],p[2]);
 gfx_ScaledSprite_NoClip((gfx_sprite_t*)p,0,46+OY,2,2);dirty=true;
 return true;
}
static void cb(void){
 int h=T/300,a;uint32_t f=bG[h];bool d;
 if(PT.room==0&&drawBG(h)){d=bb();bd(130,d?119:103,d,1);return;}
 if(PT.room<3){
  M(0,62,320,77,bNr[PT.room],1);M(0,139,320,31,bq,1);M(22,68,66,47,aA,1);M(26,72,58,39,f,0);M(54,72,3,39,G,1);M(105,80,115,55,br,1);
  if(PT.room==0){M(100,105,125,23,bJ,1);M(120,85,25,17,aw,1);}
  else if(PT.room==1){M(110,85,95,19,G,1);M(110,105,105,30,aH,1);}
  else for(a=110;a<=180;a+=35)M(a,91,30,40,bm,1);
 }else{
  M(0,62,320,67,f,0);M(0,129,320,41,PT.room==4?bI:bt,1);
  if(PT.room==5){M(65,125,180,43,G,1);M(71,130,168,33,aH,1);}
  else{static const int xs[4]={25,75,242,289};for(a=0;a<4;a++){M(xs[a],103,6,31,aA,1);M(xs[a]-9,83,24,25,PT.room==3?bM:G,1);}}
 }
 d=bb();bd(130,d?119:103,d,1);
}
static void bc(int a,const char *d,int b,int e,uint32_t c){char s[16];sprintf(s,"%s %d",d,b);At(a,20,s,102,1);U(a+4,40,94,5,aT);U(a+4,40,94*b/e,5,c);}
static void bg(void){
 char s[24],n[16];
 U(0,0,320,46,G);
 sprintf(s,"%d:%s",ae+1,PT.nm);At(4,0,s,140,0);
 aU(n,PT.lv,5);sprintf(s,"L%s",n);At(147,0,s,65,1);
 aU(n,H,9);sprintf(s,"$%s",n);At(216,0,s,100,2);
 bc(4,"HP",PT.hp,100,aR);bc(109,"JOY",PT.joy,10,aH);bc(214,"FOOD",PT.food,100,aw);
}
static void aX(void){
 aW=aj[0]!=0;U(0,156,320,54,aA);
 if(aj[0]){A(6,157,"SAVE ERROR",308,0,G);A(6,174,aj,308,0,G);A(6,191,"CLEAR Retry / Exit",308,0,G);}
 else if(L||K[0]||ad){
  char b[80],c[40];int a,n;
  U(3,156,314,54,L?a8:G);
  if(L){strcpy(b,L->txt);sprintf(c,"ENT %s / DEL Skip",L->act);}
  else if(K[0]){strcpy(b,K);strcpy(c,a2);}
  else{snprintf(b,sizeof b,"%s: %s",PT.nm,ad);strcpy(c,a2);}
  n=(int)strlen(b);a=n;
  if(a>30){for(a=30;a>0&&b[a]!=' ';a--);if(a<=0)a=30;}
  {char l1[40];memcpy(l1,b,a);l1[a]=0;At(8,157,l1,304,0);}
  if(a<n){const char *r=b+a;while(*r==' ')r++;At(8,174,r,304,0);}
  At(8,191,c,304,2);
 }else{
  char s[16];sprintf(s,"%s%s",a7[T/300],bb()?" Zz":"");
  A(6,157,s,110,0,G);A(120,157,aS[PT.room],194,2,G);A(6,174,"1-9 Switch / 0 Pets",308,1,G);A(6,191,"DOWN Clothes / STAT Guide",308,1,G);
 }
}
static void v(void){if(pfull){pn=1;pfull=false;}cb();bg();aX();}

/* ---------- keys ---------- */
enum{K_NONE,K_ENTER,K_CLEAR,K_UP,K_DOWN,K_LEFT,K_RIGHT,K_ALPHA,K_STAT,K_MODE,K_DEL,K_VARS,K_2ND,K_MATH,K_LP,K_RP,K_0,K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9};
static uint8_t lastcsc;
static uint8_t z(void){
 uint8_t k;
 if(dirty){gfx_BlitBuffer();dirty=false;}
 k=os_GetCSC();lastcsc=k;
 switch(k){
  case sk_Enter:return K_ENTER;case sk_Clear:return K_CLEAR;case sk_Up:return K_UP;case sk_Down:return K_DOWN;
  case sk_Left:return K_LEFT;case sk_Right:return K_RIGHT;case sk_Alpha:return K_ALPHA;case sk_Stat:return K_STAT;
  case sk_Mode:return K_MODE;case sk_Del:return K_DEL;case sk_Vars:return K_VARS;case sk_2nd:return K_2ND;case sk_Math:return K_MATH;
  case sk_LParen:return K_LP;case sk_RParen:return K_RP;
  case sk_0:return K_0;case sk_1:return K_1;case sk_2:return K_2;case sk_3:return K_3;case sk_4:return K_4;
  case sk_5:return K_5;case sk_6:return K_6;case sk_7:return K_7;case sk_8:return K_8;case sk_9:return K_9;
 }
 return K_NONE;
}
static bool isdig(uint8_t k){return k>=K_0&&k<=K_9;}
static void drain(void){while(os_GetCSC());}
static uint8_t af(void){uint8_t a;drain();for(;;){a=z();if(a==K_ENTER||a==K_CLEAR)return a;}}
static void D(int a,const char *b){U(0,a,320,24,G);At(6,a,b,308,1);}
static void al(const char *a){ao(32);A(6,5,a,308,1,G);bd(132,43,0,0);}
static void aY(const char *a){char s[16],n[12];ao(30);A(5,3,a,200,0,G);aU(n,H,9);sprintf(s,"$%s",n);A(210,3,s,105,2,G);}

/* ---------- saving ---------- */
static bool ab(void){
 static save_t s;ti_var_t h;size_t ok;
 memcpy(s.magic,"FOXH",4);s.ver=1;s.ae=ae;s.nF=nF;s.H=H;s.T=(uint16_t)T;
 memcpy(s.V,V,sizeof V);memcpy(s.ap,ap,sizeof ap);memcpy(s.cM,cM,sizeof cM);memcpy(s.cO,cO,sizeof cO);memcpy(s.F,F,sizeof F);
 h=ti_Open(SAVE_NAME,"w");
 if(!h){strcpy(aj,"Not enough RAM to save");strcpy(E,"Save error");I=true;return false;}
 ok=ti_Write(&s,sizeof s,1,h);ti_Close(h);
 if(ok!=1){strcpy(aj,"Not enough RAM to save");strcpy(E,"Save error");I=true;return false;}
 I=false;aj[0]=0;return true;
}
static void archiveSave(void){ti_var_t h=ti_Open(SAVE_NAME,"r");if(h){ti_SetArchiveStatus(true,h);ti_Close(h);}}
static bool b5(void){
 static save_t s;ti_var_t h;uint8_t i;bool ok;
 h=ti_Open(SAVE_NAME,"r");if(!h)return false;
 ok=ti_GetSize(h)==sizeof s&&ti_Read(&s,sizeof s,1,h)==1;ti_Close(h);
 if(!ok||memcmp(s.magic,"FOXH",4)||s.ver!=1||s.nF<1||s.nF>9||s.ae>=s.nF)return false;
 for(i=0;i<s.nF;i++){pet_t *p=&s.F[i];p->nm[12]=0;if(!p->nm[0]||p->gen>2||p->room>5||p->col>7||p->pat>3)return false;}
 nF=s.nF;ae=s.ae;H=s.H;T=s.T%1200;memcpy(V,s.V,sizeof V);memcpy(ap,s.ap,sizeof ap);memcpy(cM,s.cM,sizeof cM);memcpy(cO,s.cO,sizeof cO);memcpy(F,s.F,sizeof F);
 return true;
}

/* ---------- game logic ---------- */
static void aE(uint8_t a){ae=a;ad=NULL;L=NULL;K[0]=0;ak=0;av=0;snprintf(E,sizeof E,"Hello, %s!",PT.nm);}
static uint32_t bT(void){return 1500UL<<(nF-1);}
static const ev_t *bX(void){
 int b=(PT.room==3||PT.room==4)&&T/300==3,c=PT.room==3,a=rnd(0,3+b+c);
 if(a<4)return &EV[PT.room][a];
 if(b&&a==4)return &STAR;
 return &TRAV;
}
static void ca(void){
 char ty;uint8_t a,d;
 if(!L)return;
 ty=L->ty;a=L->amt;d=L->item;L=NULL;
 if(ty=='b'){if(rnd(1,3)==1){ty='c';a=35;}else{ty='j';a=1;}}
 if(ty=='c'){H+=a;sprintf(K,"+$%d",a);}
 else if(ty=='j'){PT.joy+=a;if(PT.joy>=10){PT.lv+=PT.joy/10;PT.joy%=10;sprintf(K,"Level up! Lv.%lu",(unsigned long)PT.lv);}else sprintf(K,"+%d joy",a);}
 else if(ty=='h'){a=(uint8_t)imin(a,100-PT.hp);PT.hp+=a;sprintf(K,"+%d HP",a);}
 else if(ty=='u'){a=(uint8_t)imin(a,100-PT.food);PT.food+=a;sprintf(K,"+%d food",a);}
 else{if(ty=='r')d=(uint8_t)rnd(0,6);V[d]+=a;sprintf(K,"Got %s!",at[d]);}
 I=true;if(!ab())strcpy(K,"Save failed! Try CLEAR");
}
static void bZ(void){
 if(PT.food>1){PT.food=(uint8_t)imax(1,PT.food-5);strcpy(E,"Getting hungry...");}
 else{PT.hp=(uint8_t)imax(1,PT.hp-3);PT.joy=(uint8_t)imax(0,PT.joy-1);strcpy(E,"Your fox needs food!");}
 I=true;
}
static uint8_t ba(uint32_t k){
 uint8_t b=0,i;uint32_t m;
 I=true;
 for(m=0;m<k;m++){
  int d,c;bool j,f;
  if(C==S_HOME){ak++;if(ak>=45){ak=0;if(!L&&!K[0]&&!ad&&rnd(1,3)==1){L=bX();if(!b)b=1;}}}
  av++;if(av>=30){av=0;if(!ad&&!L&&!K[0]&&rnd(1,3)==1){ad=bB[rnd(0,19)];if(!b)b=1;}}
  d=T/300;j=d==3&&PT.wake==0;T=(T+1)%1200;PT.hung++;
  if(PT.hung>=120){PT.hung=0;bZ();if(!b)b=1;}
  if(PT.wake>0)PT.wake--;
  c=T/300;
  if(c!=d){PT.wake=0;PT.rest=0;b=2;if(c==3)strcpy(E,"Good night!");else sprintf(E,"Good %s!",a7l[c]);}
  f=c==3&&PT.wake==0;
  if(f!=j){PT.rest=0;b=2;if(f)strcpy(E,"Your fox is sleeping");}
  if(f&&j){PT.rest++;if(PT.rest>=15){PT.rest=0;if(PT.hp<100){PT.hp=(uint8_t)imin(100,PT.hp+2);if(!b)b=1;strcpy(E,"Resting: +2 HP");}}}
  else if(!f)PT.rest=0;
  for(i=0;i<nF;i++){
   pet_t *a=&F[i];bool h;
   if(i==ae)continue;
   a->hung++;
   if(a->hung>=120){a->hung=0;if(a->food>1)a->food=(uint8_t)imax(1,a->food-5);else{a->hp=(uint8_t)imax(1,a->hp-3);a->joy=(uint8_t)imax(0,a->joy-1);}}
   h=d==3&&a->wake==0;
   if(a->wake>0)a->wake--;
   if(c!=d){a->wake=0;a->rest=0;}
   if(c==3&&a->wake==0&&h){a->rest++;if(a->rest>=15){a->rest=0;a->hp=(uint8_t)imin(100,a->hp+2);}}
   else a->rest=0;
  }
 }
 return b;
}
static bool b8(void){
 bool a=bb();
 if(T/300==3){PT.wake=60;PT.rest=0;}
 H++;ax++;I=true;
 if(rnd(1,30)==1){PT.joy++;strcpy(E,"+$1 and +1 joy!");if(PT.joy>=10){PT.joy=0;PT.lv++;sprintf(E,"LEVEL UP! Lv.%lu",(unsigned long)PT.lv);}}
 return a;
}

/* ---------- name entry (replaces Python input) ---------- */
static const uint8_t lk[26]={sk_Math,sk_Apps,sk_Prgm,sk_Recip,sk_Sin,sk_Cos,sk_Tan,sk_Power,sk_Square,sk_Comma,sk_LParen,sk_RParen,sk_Div,sk_Log,sk_7,sk_8,sk_9,sk_Mul,sk_Ln,sk_4,sk_5,sk_6,sk_Sub,sk_Store,sk_1,sk_2};
static bool nameEntry(char *out,bool cancel){
 char s[13]="";uint8_t n=0,i,k;bool lower=false,redraw=true;
 drain();
 for(;;){
  if(redraw){
   char t[20];
   ao(32);A(6,5,"NAME YOUR FOX",308,1,G);
   U(40,60,240,30,a9);s[n]=0;sprintf(t,"%s_",s);At(48,63,t,224,0);
   At(6,105,"Letter keys type (like ALPHA)",308,1);
   At(6,125,lower?"ALPHA: lowercase (on)":"ALPHA: lowercase (off)",308,1);
   At(6,145,"0 Space  DEL Erase",308,1);
   At(6,175,cancel?"ENTER Done / CLEAR Cancel":"ENTER Done",308,1);
   redraw=false;
  }
  s[n]=0;z();k=lastcsc;
  if(!k)continue;
  if(k==sk_Enter){if(!n)strcpy(out,"Fox");else{s[n]=0;strcpy(out,s);}return true;}
  if(k==sk_Clear&&cancel)return false;
  if(k==sk_Alpha){lower=!lower;redraw=true;continue;}
  if(k==sk_Del){if(n)n--;redraw=true;continue;}
  if(n>=12)continue;
  if(k==sk_0){s[n++]=' ';redraw=true;continue;}
  for(i=0;i<26;i++)if(lk[i]==k){s[n++]=(char)((lower?'a':'A')+i);redraw=true;break;}
 }
}
static int numEntry(const char *title){
 char s[10];uint8_t n=0,k;bool redraw=true;
 drain();
 for(;;){
  if(redraw){ao(32);A(6,5,title,308,1,G);U(40,60,240,30,a9);s[n]=0;At(48,63,n?s:"_",224,0);At(6,175,"Numbers / ENTER / CLEAR",308,1);redraw=false;}
  k=z();
  if(k==K_ENTER)return n?atoi(s):0;
  if(k==K_CLEAR)return -1;
  if(k==K_DEL&&n){n--;redraw=true;}
  if(isdig(k)&&n<9){s[n++]=(char)('0'+k-K_0);redraw=true;}
 }
}
static int bP(bool g){
 int b=0;bool e=true;uint8_t a,c;
 drain();
 for(;;){
  if(e){ao(34);A(6,7,"SELECT FOX GENDER",308,1,G);for(c=0;c<3;c++){int f=49+c*36;an(30,f,260,30,c==b);At(36,f+5,a1[c],248,1);}At(6,173,"UP/DOWN / ENTER",308,1);e=false;}
  a=z();
  if(a==K_ENTER)return b;
  if(a==K_CLEAR&&g)return -1;
  if(a==K_UP||a==K_DOWN){b=(b+(a==K_UP?2:1))%3;e=true;}
 }
}

/* ---------- menus ---------- */
static void b9(void){
 char s[40];int b=nF,c=imax(0,imin(B-2,b-3)),a;
 sprintf(s,"YOUR FOXES %d/9",nF);aY(s);
 for(a=c;a<imin(b,c+4);a++){int e=32+(a-c)*30;an(5,e,310,28,a==B);sprintf(s,"%d: %s%s",a+1,F[a].nm,a==ae?" *":"");At(8,e,s,155,0);sprintf(s,"HP%d F%d",F[a].hp,F[a].food);At(165,e,s,145,2);}
 if(b<9){an(5,153,310,24,B==b);sprintf(s,"Adopt: $%lu",(unsigned long)bT());At(8,154,s,304,1);}
 else At(8,154,"All 9 foxes adopted!",304,1);
 At(5,179,E[0]?E:"ENTER Select / DEL Release",310,1);At(5,195,"CLEAR Back",310,1);
}
static void cRelease(void){
 uint8_t i,j;char s[48];
 if(B>=nF)return;
 if(nF==1){strcpy(E,"Keep at least one fox!");return;}
 ao(32);A(6,5,"RELEASE FOX",308,1,G);sprintf(s,"%s will leave forever.",F[B].nm);D(65,s);D(95,"No refund. Clothes stay yours.");D(145,"ENTER Release / CLEAR Cancel");
 if(af()!=K_ENTER)return;
 i=ae;
 for(j=B;j+1<nF;j++){F[j]=F[j+1];cO[j]=cO[j+1];}
 cO[8]=0;nF--;
 aE((uint8_t)imin(i-(B<i),nF-1));B=(uint8_t)imin(B,nF-1);I=true;
 strcpy(E,ab()?"Fox released.":"Save failed! Try CLEAR");
}
static bool bW(void){
 uint32_t e=bT();char h[13],s[40];int d;uint8_t f=0,a,b,c;pet_t *p;
 if(nF>=9||H<e){strcpy(E,nF<9?aQ:"Home is full!");return false;}
 al("ADOPT A FOX");sprintf(s,"Adopt for $%lu?",(unsigned long)e);D(105,s);D(180,"ENTER Yes / CLEAR Cancel");
 if(af()!=K_ENTER)return false;
 if(!nameEntry(h,true))return false;
 d=bP(true);if(d<0)return false;
 for(c=0;c<nF;c++)f|=(uint8_t)(1<<F[c].col);
 if(f!=255){do a=(uint8_t)rnd(0,7);while(f&(1<<a));b=(uint8_t)rnd(0,3);}
 else for(;;){bool g=false;a=(uint8_t)rnd(0,7);b=(uint8_t)rnd(0,3);if(a==F[nF-1].col)continue;for(c=0;c<nF;c++)if(F[c].col==a&&F[c].pat==b){g=true;break;}if(!g)break;}
 p=&F[nF];memset(p,0,sizeof *p);strcpy(p->nm,h);p->gen=(uint8_t)d;p->hp=100;p->lv=1;p->food=100;p->room=PT.room;p->col=a;p->pat=b;
 cO[nF]=0;nF++;H-=e;aE(nF-1);I=true;ab();
 al("WELCOME HOME!");sprintf(s,"%s - %s",h,a1[d]);D(105,s);sprintf(s,"%s / %s",bk[a],bD[b]);D(130,s);D(180,"ENTER Continue");af();
 return true;
}
static void bY(void){
 char s[16];int i;
 ao(28);sprintf(s,"GUIDE %d/4",B+1);A(6,3,s,308,1,G);
 if(B<3){for(i=0;i<7;i++)if(bl[B][i][0])At(6,30+i*21,bl[B][i],308,0);}
 else{At(6,55,VERSION,308,1);At(6,95,"Created with love by Kinny",308,1);At(6,125,"CE C port",308,1);}
 At(6,185,"LEFT/RIGHT Page  CLEAR Back",308,1);
}
static void cName(char *o,int j,int n){
 if(!n){strcpy(o,"None");return;}
 if(j==0){strcpy(o,cH[n-1]);return;}
 if(j==1){sprintf(o,"%s %s",cN[(n-1)%9],n<=9?"sweater":"hoodie");return;}
 if(n<=5)strcpy(o,cA[n-1]);else if(n<15)sprintf(o,"%s scarf",cN[n-6]);else strcpy(o,"Sunglasses");
}
static int cPrice(int j,int n){static const int hp[6]={120,500,180,90,100,300};return j==0?hp[n-1]:j==1?(n<=9?150:220):(n<=5?100:n<15?80:160);}
static void cMenu(void){
 char s[32];int a=cO[ae]>>cS[cC]&cB[cC],b=imax(0,imin(B-1,cL[cC]-4)),i;
 aY(cT[cC]);At(5,30,"LEFT/RIGHT Category",310,1);
 for(i=b;i<imin(cL[cC]+1,b+5);i++){
  int d=50+(i-b)*25;an(4,d,208,24,i==B);cName(s,cC,i);At(7,d+1,s,200,0);
  if(i==a)strcpy(s,"ON");else if(!i||cM[cC]&(1UL<<(i-1)))strcpy(s,"OWNED");else sprintf(s,"$%d",cPrice(cC,i));
  At(216,d+1,s,100,2);
 }
 At(5,177,E[0]?E:(B?"ENTER Equip / Buy":"ENTER Remove"),310,1);At(5,191,"CLEAR Back",310,1);
}
static void cKey(uint8_t a){
 if(a==K_CLEAR){C=S_HOME;v();return;}
 if(a==K_LEFT||a==K_RIGHT){cC=(uint8_t)((cC+(a==K_LEFT?2:1))%3);B=0;E[0]=0;}
 else if(a==K_UP||a==K_DOWN){int m=cL[cC]+1;B=(uint8_t)((B+(a==K_UP?m-1:1))%m);E[0]=0;}
 else if(a==K_ENTER){
  if(B&&!(cM[cC]&(1UL<<(B-1)))){int p=cPrice(cC,B);if(H<(uint32_t)p){strcpy(E,aQ);cMenu();return;}H-=p;cM[cC]|=1UL<<(B-1);}
  cO[ae]=(uint16_t)((cO[ae]&~(cB[cC]<<cS[cC]))|(B<<cS[cC]));I=true;strcpy(E,B?"Equipped!":"Removed!");
  if(!ab())strcpy(E,"Save failed!");
 }else return;
 cMenu();
}
static void cDev(void){
 for(;;){
  char s[32];int a,n,j;
  ao(32);sprintf(s,"DEV: %s",PT.nm);A(6,5,s,308,1,G);
  D(50,"1 Cash  2 Level  3 HP");D(75,"4 Joy  5 Food  6 Bag");D(100,"7 Unlock clothes");D(125,"0 Exit");
  drain();do a=z();while(!isdig(a)&&a!=K_CLEAR);
  if(a==K_CLEAR||a==K_0)break;
  a-=K_0;
  if(a==7){for(j=0;j<3;j++)cM[j]=(1UL<<cL[j])-1;}
  else if(a<=6){n=numEntry("VALUE");if(n<0)continue;
   if(a==1)H=(uint32_t)n;else if(a==2)PT.lv=(uint32_t)imax(1,n);else if(a==3)PT.hp=(uint8_t)imax(1,imin(100,n));
   else if(a==4)PT.joy=(uint8_t)imin(9,n);else if(a==5)PT.food=(uint8_t)imax(1,imin(100,n));else for(j=0;j<7;j++)V[j]=(uint32_t)n;}
  else continue;
  I=true;
 }
 if(I)ab();
 E[0]=0;
}
static void W(void){
 int a,b,c;char s[24],n[16];
 if(C==S_CONTROLS)bY();
 else if(C==S_PETS)b9();
 else if(C==S_CLOTHES)cMenu();
 else if(C==S_GAMES){
  ao(32);A(6,5,"MINIGAMES",308,1,G);
  for(a=0;a<3;a++){b=36+a*42;an(8,b,304,40,a==B);At(15,b+2,gName[a],290,0);At(15,b+21,gDesc[a],290,0);}
  At(6,164,"Play with your fox!",308,1);At(6,187,"ENTER Play / CLEAR Back",308,1);
 }else if(C==S_ROOMS){
  ao(28);A(8,3,"CHOOSE A ROOM",304,1,G);
  for(a=0;a<6;a++){b=28+a*22;an(8,b,304,21,a==B);At(14,b+1,aS[a],230,0);if(a==PT.room)At(250,b+1,"*",52,2);}
  At(8,185,"ENTER Go / CLEAR Back",304,1);
 }else{
  bool sh=C==S_SHOP;
  ao(30);A(8,5,sh?"FOOD SHOP":"FOOD BAG",160,0,G);aU(n,H,12);sprintf(s,"$%s",n);A(174,5,s,138,2,G);
  At(8,30,"FOOD",138,0);At(151,30,sh?"PRICE":"QTY",66,1);At(225,30,"FILL",85,1);c=imax(0,imin(B-2,3));
  for(a=c;a<c+4;a++){
   b=51+(a-c)*28;an(4,b,312,26,a==B);At(8,b+3,at[a],143,0);
   if(sh)sprintf(s,"$%d",aO[a]);else{aU(n,V[a],6);sprintf(s,"x%s",n);}
   A(153,b+3,s,63,1,bE);U(231,b+2,78,22,aw);sprintf(s,"+%d",a0[a]);A(233,b+3,s,74,1,0);
  }
  if(E[0])At(8,164,E,304,1);else{sprintf(s,"%d/7 UP/DOWN",B+1);At(8,164,s,304,1);}
  At(8,187,a3,304,1);
 }
}

/* ---------- minigames ---------- */
static bool be(uint32_t t){uint32_t a=ms();while(ms()-a<t)if(z()==K_CLEAR)return false;return true;}
static void aC(const char *f,bool b,const char *e){
 char a[32]="",s[48];bool d=true;
 if(b){if(rnd(0,1)==0){H+=50;strcpy(a,"$50");}else{int c=rnd(0,6);V[c]++;strcpy(a,at[c]);}I=true;d=ab();}
 al(f);D(96,b?"YOU WIN!":"Try again!");D(121,e);
 if(b){sprintf(s,"Reward: %s",a);D(146,s);}else D(146,"No reward this time");
 D(184,d?"ENTER / CLEAR Back":"Save failed! ENTER Back");af();
}
static void ce(void){
 char d[10],s[24];int a,c,e;uint8_t b;
 al("SIMON SAYS");D(100,"Remember the digits!");D(126,"Repeat all 10 rounds to win");D(180,a4);
 if(af()!=K_ENTER)return;
 for(a=1;a<=10;a++){
  d[a-1]=(char)('0'+rnd(0,9));sprintf(s,"SIMON SAYS %d/10",a);al(s);D(100,"Watch your fox!");
  for(e=0;e<a;e++){sprintf(s,"%s: %c",PT.nm,d[e]);D(130,s);if(!be(800))return;D(130,"...");if(!be(300))return;}
  D(100,"Repeat the sequence!");sprintf(s,"0/%d",a);D(130,s);D(180,"Number keys / CLEAR Back");c=0;drain();
  while(c<a){
   b=z();
   if(b==K_CLEAR)return;
   if(isdig(b)){
    if('0'+b-K_0!=d[c]){sprintf(s,"Round %d",a);aC("SIMON SAYS",false,s);return;}
    c++;sprintf(s,"%d/%d",c,a);D(130,s);
   }
  }
  D(100,"Correct!");if(!be(650))return;
 }
 aC("SIMON SAYS",true,"All 10 rounds complete!");
}
static void cf(void){
 int a=0,f=-1,g=-1,dd;uint32_t h,b,c=0;bool first=true;uint8_t e;char s[32];
 al("SUMO");D(100,"50 ENTER taps in 10 seconds!");D(126,"Tap fast to beat your fox!");D(180,a4);
 if(af()!=K_ENTER)return;
 drain();D(100,"GO! Spam ENTER!");D(180,"CLEAR Back");h=ms();
 for(;;){
  e=z();b=ms()-h;
  if(e==K_CLEAR)return;
  if(b>=10000)break;
  if(e==K_ENTER&&(first||b-c>=25)){first=false;a++;c=b;if(a>=50)break;}
  dd=10-(int)(b/1000);
  if(dd!=f||a/5!=g){f=dd;g=a/5;sprintf(s,"%d/50  |  %ds",a,dd);D(126,s);U(20,155,280,10,aT);U(20,155,280*a/50,10,aw);}
 }
 sprintf(s,"%d/50 taps",a);aC("SUMO",a>=50,s);
}
static void b1(void){
 uint32_t c,f;uint8_t b;int d,a;char s[32];bool e;
 al("FISHING");D(100,"Wait for BITE!");D(126,"Then tap ENTER quickly");D(180,"ENTER Cast / CLEAR Back");
 if(af()!=K_ENTER)return;
 drain();f=(uint32_t)rnd(20,65)*100;D(100,"Waiting for a fish...");U(0,126,320,45,bO);U(155,132,8,12,aR);c=ms();
 while(ms()-c<f){b=z();if(b==K_CLEAR)return;if(b==K_ENTER){aC("FISHING",false,"Too early!");return;}}
 D(100,"BITE! ENTER NOW!");c=ms();
 while(ms()-c<900){
  b=z();
  if(b==K_CLEAR)return;
  if(b==K_ENTER){
   d=rnd(1,100);a=0;while(d>aM[a]){d-=aM[a];a++;}
   ap[a]++;H+=aN[a];V[2]++;I=true;e=ab();al("FISH CAUGHT!");D(96,aZ[a]);sprintf(s,"+$%d and 1 Fish",aN[a]);D(121,s);
   sprintf(s,"Caught: %lu",(unsigned long)ap[a]);D(146,s);D(184,e?"ENTER Back":"Save failed!");af();return;
  }
 }
 aC("FISHING",false,"The fish escaped!");
}

/* ---------- main loop ---------- */
static void cc(void){
 uint32_t d,c,f,h,e;uint8_t a,b,u=0;bool l=b5();
 if(!l){
  pet_t *p=&F[0];char nm[13];int g;
  nameEntry(nm,false);g=bP(false);
  memset(F,0,sizeof F);strcpy(p->nm,nm);p->gen=(uint8_t)g;p->hp=100;p->lv=1;p->food=100;nF=1;ae=0;
  ab();
 }
 aE(ae);snprintf(E,sizeof E,"Welcome, %s!",PT.nm);C=S_HOME;v();d=ms();f=d+30000;
 for(;;){
  a=z();c=ms();b=0;
  h=c-d;if(h>=1000){e=h/1000;d+=e*1000;b=ba(e);}
  if(a){
   if(C==S_HOME){
    if(a==K_LP||a==K_RP){
     static const uint8_t seq[4]={K_LP,K_RP,K_LP,K_RP};
     u=a==seq[u]?u+1:(a==K_LP);
     if(u==4){u=0;cDev();v();d=ms()-(c-d);f=ms()+30000;}
     continue;
    }
    u=0;
    if(a==K_CLEAR){
     if(ab())return;
     al("SAVE ERROR");D(105,aj);D(135,"ENTER Retry / CLEAR Exit");
     if(af()==K_CLEAR)return;
     if(ab())return;
     v();
    }
    else if(a==K_STAT){C=S_CONTROLS;B=0;W();}
    else if(a==K_0){C=S_PETS;B=ae;E[0]=0;W();}
    else if(a>=K_1&&a<=K_9){uint8_t j=a-K_1;if(j<nF&&j!=ae){aE(j);I=true;v();b=0;}}
    else if(a==K_DEL){if(L)L=NULL;else if(K[0])K[0]=0;else ad=NULL;aX();}
    else if(a==K_DOWN){C=S_CLOTHES;B=0;E[0]=0;cMenu();}
    else if(a==K_MATH){C=S_GAMES;B=0;W();}
    else if(a==K_VARS||a==K_2ND){C=S_ROOMS;B=PT.room;W();}
    else if(a==K_ALPHA||a==K_LEFT){C=S_SHOP;B=0;E[0]=0;W();}
    else if(a==K_MODE||a==K_RIGHT){C=S_BAG;B=0;E[0]=0;W();}
    else if(a==K_ENTER){
     bool m;
     if(L){ca();v();continue;}
     m=b8();
     if(m||b==2)v();else{bg();if(b||aW!=(aj[0]!=0))aX();}
     b=0;
    }
   }
   else if(C==S_CLOTHES){cKey(a);b=0;}
   else if(C==S_CONTROLS){
    if(a==K_CLEAR||a==K_STAT){C=S_HOME;v();b=0;}
    else if(a==K_LEFT||a==K_RIGHT||a==K_UP||a==K_DOWN){B=(uint8_t)((B+((a==K_LEFT||a==K_UP)?3:1))%4);W();}
   }
   else if(C==S_PETS){
    if(a==K_CLEAR||a==K_0){C=S_HOME;v();b=0;}
    else if(a==K_DEL){cRelease();W();b=0;d=ms()-(c-d);f=ms()+30000;}
    else if(a==K_UP||a==K_DOWN){int m=nF+(nF<9);B=(uint8_t)((B+(a==K_UP?m-1:1))%m);E[0]=0;W();}
    else if(a==K_ENTER){
     if(B<nF){aE(B);I=true;C=S_HOME;v();b=0;}
     else if(bW()){C=S_HOME;v();b=0;}
     else W();
     d=ms()-(c-d);
    }
   }
   else if(a==K_CLEAR||(C==S_GAMES&&a==K_MATH)||(C==S_SHOP&&a==K_ALPHA)||(C==S_BAG&&a==K_MODE)||(C==S_ROOMS&&(a==K_VARS||a==K_2ND))){C=S_HOME;E[0]=0;v();b=0;}
   else if(a==K_UP||a==K_DOWN){int m=C==S_GAMES?3:C==S_ROOMS?6:7;B=(uint8_t)((B+(a==K_UP?m-1:1))%m);E[0]=0;W();}
   else if(a==K_ENTER){
    if(C==S_GAMES){
     if(B==0)ce();else if(B==1)cf();else b1();
     c=ms();e=(c-d)/1000;if(e){d+=e*1000;ba(e);}
     W();continue;
    }
    if(C==S_ROOMS){if(PT.room!=B){L=NULL;K[0]=0;ak=0;}PT.room=B;I=true;C=S_HOME;v();continue;}
    if(C==S_SHOP){if(H>=aO[B]){H-=aO[B];V[B]++;sprintf(E,"Bought %s!",at[B]);I=true;}else strcpy(E,aQ);W();}
    else if(V[B]>0&&PT.food<100){
     if(rnd(1,4)==1)ad="I don't want that, got anything else? :3";
     else{V[B]--;PT.food=(uint8_t)imin(100,PT.food+a0[B]);I=true;sprintf(E,"Yum! %s",at[B]);}
     C=S_HOME;v();b=0;
    }
    else{strcpy(E,PT.food>=100?"Already full!":"None in your bag!");W();}
   }
  }
  if(b&&C==S_HOME){if(b==2)v();else{bg();aX();}}
  if(c>=f){if(I)ab();f=c+30000;}
 }
}

int main(void){
 ti_var_t h;
 srand((unsigned)rtc_Time());
 h=ti_Open(BG_NAME,"r");
 if(h){if(ti_GetSize(h)==4+4*(384+2+160*55)){bgdata=(const uint8_t*)ti_GetDataPtr(h);if(memcmp(bgdata,"FBG1",4))bgdata=NULL;}ti_Close(h);}
 gfx_Begin();
 gfx_SetDrawBuffer();
 gfx_SetMonospaceFont(8);
 gfx_SetTextTransparentColor(0);gfx_SetTextBGColor(0);
 gfx_FillScreen(ci(aA));
 cc();
 gfx_End();
 archiveSave();
 return 0;
}
