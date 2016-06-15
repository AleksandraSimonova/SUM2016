/* FILE NAME: RENDER.C
 * PROGRAMMER: SA2
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include "anim.h"

/* Global transformation matrices */
MATR
  SA2_RndMatrWorld, /* World (object) transformation matrix */
  SA2_RndMatrView,  /* Camera view transform */
  SA2_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  SA2_RndProjDist, /* Near clip plane */
  SA2_RndFarClip,  /* Far clip plane */
  SA2_RndProjSize; /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID SA2_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (SA2_Anim.W >= SA2_Anim.H)
    ratio_x = (DBL)SA2_Anim.W / SA2_Anim.H;
  else
    ratio_y = (DBL)SA2_Anim.H / SA2_Anim.W;

  SA2_RndMatrProj = MatrFrustum(-ratio_x * SA2_RndProjSize / 2,
                                 ratio_x * SA2_RndProjSize / 2,
                                -ratio_y * SA2_RndProjSize / 2,
                                 ratio_y * SA2_RndProjSize / 2,
                                 SA2_RndProjDist, SA2_RndFarClip);
} /* End of 'SA2_RndSetProj' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *      sa2PRIM *Pr;
 * RETURNS: None.
 */
VOID SA2_RndPrimDraw( sa2PRIM *Pr )
{
  INT i;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(SA2_RndMatrWorld,
    MatrMulMatr(SA2_RndMatrView, SA2_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.R);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'SA2_RndPrimDraw' function */

VOID SA2_RndPrimFree( sa2PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(sa2PRIM));
} /* End of 'VG4_RndPrimFree' function */


/* END OF 'RENDER.C' FILE */
