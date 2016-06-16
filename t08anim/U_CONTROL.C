/* file name: U_CONTROL.C
 *progrAmmer: sa2
 *date: 15.06.2016
 */
#include <string.h>                   



#include "anim.h"
#include "units.h"

/* Ball unit representation type */
typedef struct
{
  sa2UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  
} sa2UNIT_CONTROL;

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       sa2UNIT_CUBE *Uni;
 *   - animation context:
 *       sa2ANIM *Ani;
 * RETURNS: None.
 */
static VOID SA2_UnitInit( sa2UNIT_CONTROL *Uni, sa2ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 3);
} /* End of 'SA2_UnitInit' function */


/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       sa2UNIT_CUBE *Uni;
 *   - animation context:
 *       sa2ANIM *Ani;
 * RETURNS: None.
 */
static VOID SA2_UnitResponse( sa2UNIT_CONTROL *Uni, sa2ANIM *Ani )
{
   DBL r;

  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani->Keys[VK_SPACE])
    SA2_AnimAddUnit(SA2_UnitCreateBall());
  if (Ani->Keys['C'])
    SA2_AnimAddUnit(SA2_UnitCreateCube());
           
  if (Ani->KeysClick[VK_ESCAPE])
    SA2_AnimDoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
 /* Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateX(59 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateX(59 * Ani->Mdy * Ani->GlobalDeltaTime));
  }  */

  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(59 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(-59 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->DeltaTime * 0.1) / r);  

  SA2_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));

}
sa2UNIT * SA2_UnitCreateControl( VOID )
{
  sa2UNIT_CONTROL *Uni;

  if ((Uni = (sa2UNIT_CONTROL *)SA2_AnimUnitCreate(sizeof(sa2UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)SA2_UnitInit;

  Uni->Response = (VOID *)SA2_UnitResponse;
 
  return (sa2UNIT *)Uni;
} /* End of 'SA2_UnitCreateCube' function */

/* End of 'SA2_UnitResponse' function */

