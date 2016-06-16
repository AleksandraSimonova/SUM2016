ANIM.H:

/***
 * Render support
 ***/

/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} vg4VERTEX;

/* Primitive representation type */
typedef struct
{
  /* OpenGL buffers */
  INT
    VA,   /* Vertex array */
    VBuf, /* Vertex buffer */
    IBuf; /* Index buffer */
  INT NumOfI;       /* Facets index array size */
  MATR M; /* Primitive transformation matrix */
  INT MtlNo; /* Material number */
} vg4PRIM;

/* Object representation type */
typedef struct
{
  vg4PRIM *Prims;
  INT NumOfPrims;
} vg4OBJ;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} vg4MTL;

/* Global transformation matrices */
extern MATR
  VG4_RndMatrWorld, /* World (object) transformation matrix */
  VG4_RndMatrView,  /* Camera view transform */
  VG4_RndMatrProj;  /* Projection transform */

/* Projection parameters */
extern DBL
  VG4_RndProjDist, /* Near clip plane */
  VG4_RndFarClip,  /* Far clip plane */
  VG4_RndProjSize; /* Project plane size */

/* Shader support */
extern UINT VG4_RndPrg;

/* Materials array */
#define VG4_MAX_MATERIALS 10000
extern vg4MTL VG4_RndMaterials[VG4_MAX_MATERIALS];
extern INT VG4_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndSetProj( VOID );

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjDraw( vg4OBJ *Obj );

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL VG4_RndObjLoad( vg4OBJ *Obj, CHAR *FileName );

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID VG4_RndObjFree( vg4OBJ *Obj );

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT VG4_RndFindMaterial( CHAR *Name );

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID VG4_RndLoadMaterials( CHAR *FileName );

ANIM.C:
Init:
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ...
