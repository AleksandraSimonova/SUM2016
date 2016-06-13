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
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(SA2_RndMatrWorld,
    MatrMulMatr(SA2_RndMatrView, SA2_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = VecMulMatr(Pr->P[i], M);

    pts[i].x = (p.X + 1) * SA2_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * SA2_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(SA2_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(SA2_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'SA2_RndPrimDraw' function */

/* END OF 'RENDER.C' FILE */
