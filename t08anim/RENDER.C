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

/* Materials array */
sa2MTL SA2_RndMaterials[SA2_MAX_MATERIALS];
INT SA2_RndNumOfMaterials;

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

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       sa2OBJ *Obj;
 * RETURNS: None.
 */
VOID SA2_RndObjDraw( sa2OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)                                     
  {
    /* Build transform matrix */
    MSave = SA2_RndMatrWorld;
    SA2_RndMatrWorld = MatrMulMatr(SA2_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(SA2_RndMatrWorld,
      MatrMulMatr(SA2_RndMatrView, SA2_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    
    /*glBegin(GL_LINES);
      glColor3d(1, 0, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(1, 0, 0, 0);
      glColor3d(0, 1, 0);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 1, 0, 0);
      glColor3d(0, 0, 1);
      glVertex3d(0, 0, 0);
      glVertex4d(0, 0, 1, 0);
    glEnd(); */
   

    glUseProgram(SA2_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (SA2_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, SA2_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(SA2_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(SA2_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
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
    }

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
    if ((loc = glGetUniformLocation(SA2_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    SA2_RndMatrWorld = MSave;
  }
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
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(sa2OBJ));
} /* End of 'SA2_RndObjFree' function */

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT SA2_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < SA2_RndNumOfMaterials; i++)
    if (strcmp(Name, SA2_RndMaterials[i].Name) == 0) 
      return i;
  return -1;
} /* End of 'SA2_RndFindMaterial' function */

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID SA2_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials;
  UINT t;
  DWORD Sign;
  FILE *F;
  sa2MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (SA2_RndNumOfMaterials >= SA2_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(sa2MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /*Allocate memory for texture image*/ 
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
    }
    /* Add material to animation and OpenGL */
    SA2_RndMaterials[SA2_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'SA2_RndLoadMaterials' function */

/* END OF 'RENDER.C' FILE */
