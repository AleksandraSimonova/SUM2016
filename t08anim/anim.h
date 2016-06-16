 /* file name: anim.h
 *progremmer: sa2
 *date: 15.06.2016
 */
#ifndef  __ANIM_H_
#define  __ANIM_H_
#include "def.h"

#define MAX_UNITS 1000
#define SA2_MAX_MATERIALS 10000

typedef struct tagsa2UNIT sa2UNIT;

 
typedef struct tagsa2ANIM
{
  HWND hWnd; 
  HDC hDC; 
  HGLRC hGLRC;

  INT W, H;
  HBITMAP hFrame; 
  sa2UNIT *Units[MAX_UNITS]; 
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
  BOOL IsPause;
  INT 
    Mx, My, Mz, Mdx, Mdy, Mdz;
  BYTE 
    Keys[256],
    OldKeys[256],
    KeysClick[256];
  BYTE
    JBut[32];
  INT JPov;
  DBL
     JX, JY, JZ, JR;

  
} sa2ANIM;
struct tagsa2UNIT
{
  VOID (* Init)(sa2UNIT *Uni, sa2ANIM *Ani);
  VOID (* Close)(sa2UNIT *Uni, sa2ANIM *Ani);
  VOID (* Response)(sa2UNIT *Uni, sa2ANIM *Ani);
  VOID (* Render)(sa2UNIT *Uni, sa2ANIM *Ani);
} ;
__inline DBL Rnd0( VOID )
{
  return (DBL)rand() / RAND_MAX;
} 
__inline DBL Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
}
extern INT SA2_MouseWheel;


extern sa2ANIM SA2_Anim;
VOID  SA2_AnimInit( HWND hWnd );
VOID  SA2_AnimClose( VOID );
VOID  SA2_AnimResize( INT W, INT H );
VOID  SA2_AnimCopyFrame( VOID );
VOID  SA2_AnimRender( VOID );
VOID  SA2_AnimAddUnit( sa2UNIT *Uni );
VOID SA2_AnimDoExit( VOID );
VOID SA2_AnimFlipFullScren( VOID );
sa2UNIT * SA2_AnimUnitCreate( INT Size );
/* Global transformation matrices */
extern MATR 
      SA2_RndMatrWorld,
      SA2_RndMatrView,
      SA2_RndMatrProj;
/* Projection parameters */
extern DBL
  SA2_RndProjDist, /* Near clip plane */
  SA2_RndFarClip,  /* Far clip plane */
  SA2_RndProjSize; /* Project plane size */
/* Shader support */
extern UINT SA2_RndPrg;
/* Materials array */


typedef struct
{
  VEC  P;   /*Vertex position*/
  VEC2 T;   /*Vertex texture coordinates*/            
  VEC  N;   /*Normal at vertex*/ 
  sa2COLOR C;   /*Vertex color*/
} sa2VERTEX;

typedef struct
{
  INT VA;   /* Vertex array */
  INT VBuf; /* Vertex buffer */
  INT IBuf; /* Index buffer */
  INT NumOfI;  /* Facets index array size */
  MATR M; /* Primitive transformation matrix */
  INT MtlNo; /* Material number */
} sa2PRIM;
typedef struct
{
  sa2PRIM *Prims;
  INT NumOfPrims;
} sa2OBJ;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} sa2MTL;

#define SA2_MAX_MATERIALS 10000
extern sa2MTL SA2_RndMaterials[SA2_MAX_MATERIALS];
extern INT SA2_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID SA2_RndSetProj( VOID );

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       sa2OBJ *Obj;
 * RETURNS: None.
 */
VOID SA2_RndObjDraw( sa2OBJ *Obj );

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       sa2OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL SA2_RndObjLoad( sa2OBJ *Obj, CHAR *FileName );

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       sa2OBJ *Obj;
 * RETURNS: None.
 */
VOID SA2_RndObjFree( sa2OBJ *Obj );

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT SA2_RndFindMaterial( CHAR *Name );

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID SA2_RndLoadMaterials( CHAR *FileName );

extern INT S2_Anim;


#endif
    

