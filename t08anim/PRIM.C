/* FILE NAME: PRIM.C
 * PROGRAMMER: SA2
 * DATE: 18.06.2016
 * PURPOSE: Object handle functions.
 */

#include "anim.h"

/* Primitive create function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *      sa2PRIM *Pr;
 *   - vertex array:
 *       sa2VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - INT array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID SA2_RndPrimCreate( sa2PRIM *Pr, sa2VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(sa2PRIM));

  Pr->M = MatrIdentity();
  Pr->NumOfI = NumOfI;

  /* Create OpenGL buffers */
  glGenVertexArrays(1, &Pr->VA);
  glGenBuffers(1, &Pr->VBuf);
  glGenBuffers(1, &Pr->IBuf);

  /* Activate vertex array */
  glBindVertexArray(Pr->VA);
  /* Activate vertex buffer */
  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
  /* Store vertex data */
  glBufferData(GL_ARRAY_BUFFER, sizeof(sa2VERTEX) * NumOfV, V, GL_STATIC_DRAW);

  /* Setup data order */
  /*                    layout,
   *                      components count,
   *                          type
   *                                    should be normalize,
   *                                           vertex structure size in bytes (stride),
   *                                               offset in bytes to field start */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(sa2VERTEX),
                        (VOID *)0); /* position */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(sa2VERTEX),
                        (VOID *)sizeof(VEC)); /* texture coordinates */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(sa2VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(sa2VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

  /* Enable used attributes */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* Indices */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

  /* Disable vertex array */
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
} /* End of 'VG4_RndPrimCreate' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       sa2PRIM *Pr;
 * RETURNS: None.
 */
VOID SA2_RndPrimDraw( sa2PRIM *Pr )
{
  INT loc, mtl;
  MATR M, MSave;

  /* Build transform matrix */
  MSave = SA2_RndMatrWorld;
  SA2_RndMatrWorld = MatrMulMatr(SA2_RndMatrWorld, Pr->M);
  M = MatrMulMatr(SA2_RndMatrWorld, MatrMulMatr(SA2_RndMatrView, SA2_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Enable base shader */
  glUseProgram(SA2_RndPrg);

  mtl = Pr->MtlNo;
  if (mtl == -1)
    mtl = 0;
  if (SA2_RndMaterials[mtl].TexNo != 0)
  {
    glBindTexture(GL_TEXTURE_2D, SA2_RndMaterials[mtl].TexNo);
    if ((loc = glGetUniformLocation(SA2_RndPrg, "IsTexture")) != -1)
      glUniform1i(loc, 1);
  }
  else
    if ((loc = glGetUniformLocation(SA2_RndPrg, "IsTexture")) != -1)
      glUniform1i(loc, 0);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Ka")) != -1)
    glUniform3fv(loc, 1, &SA2_RndMaterials[mtl].Ka.X);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Kd")) != -1)
    glUniform3fv(loc, 1, &SA2_RndMaterials[mtl].Kd.X);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Ks")) != -1)
    glUniform3fv(loc, 1, &SA2_RndMaterials[mtl].Ks.X);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Ph")) != -1)
    glUniform1f(loc, SA2_RndMaterials[mtl].Ph);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Trans")) != -1)
    glUniform1f(loc, SA2_RndMaterials[mtl].Trans);
  if ((loc = glGetUniformLocation(SA2_RndPrg, "Id")) != -1)
    glUniform1i(loc, Pr->Id);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(SA2_RndPrg, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
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

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);;
  glUseProgram(0);
  SA2_RndMatrWorld = MSave;
} /* End of 'SA2_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       sa2PRIM *Pr;
 * RETURNS: None.
 */
VOID SA2_RndPrimFree( sa2PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(sa2PRIM));
} /* End of 'SA2_RndPrimFree' function */

/* END OF 'PRIM.C' FILE */
