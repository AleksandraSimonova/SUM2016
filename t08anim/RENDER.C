/* FILE NAME: RENDER.C
 * PROGRAMMER: SA2
 * DATE: 15.06.2016
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

UINT SA2_RndPrg = 0;

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
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(SA2_RndMatrWorld,
    MatrMulMatr(SA2_RndMatrView, SA2_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(SA2_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(SA2_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, SA2_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, SA2_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, SA2_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Time")) != -1)
    glUniform1f(loc, SA2_Anim.Time);


  /* Activete primitive vertex array */            
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'SA2_RndPrimDraw' function */

VOID SA2_RndPrimFree( sa2PRIM *Pr )
{
  glBindVertexArray(Pr->VA);                         
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(sa2PRIM));
} /* End of 'VG4_RndPrimFree' function */


/* END OF 'RENDER.C' FILE */
