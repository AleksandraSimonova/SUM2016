/* FILE NAME: OBJ3D.C
 * PROGRAMMER: SA2
 * DATE: 18.06.2016
 * PURPOSE: Object handle functions.
 */

#include "anim.h"

/* Object create function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID SA2_RndObjCreate( sa2OBJ *Obj )
{
  memset(Obj, 0, sizeof(sa2OBJ));
  Obj->M = MatrIdentity();
} /* End of 'SA2_RndObjCreate' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vg4OBJ *Obj;
 * RETURNS: None.
 */
VOID SA2_RndObjDraw( sa2OBJ *Obj )
{
  INT i;
  MATR MSave;

  MSave = SA2_RndMatrWorld;
  SA2_RndMatrWorld = MatrMulMatr(SA2_RndMatrWorld, Obj->M);
  for (i = 0; i < Obj->NumOfPrims; i++)
    SA2_RndPrimDraw(&Obj->Prims[i]);
  SA2_RndMatrWorld = MSave;
} /* End of 'SA2_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       sa2OBJ *Obj;
 * RETURNS: None.
 */
VOID SA2_RndObjFree( sa2OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
    SA2_RndPrimFree(&Obj->Prims[i]);
  free(Obj->Prims);
  memset(Obj, 0, sizeof(sa2OBJ));
} /* End of 'SA2_RndObjFree' function */

/* END OF 'OBJ3D.C' FILE */
