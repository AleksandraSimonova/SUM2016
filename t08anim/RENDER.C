/* FILE NAME: RENDER.C
 * PROGRAMMER: SA2
 * DATE: 15.06.2016
 * PURPOSE: Render handle functions.
 */
#include <stdio.h>
#include <string.h>
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

UINT SA2_RndPrg;


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


/* END OF 'RENDER.C' FILE */
