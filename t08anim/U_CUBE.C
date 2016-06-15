/* file name: U_BALL.C
 *progremmer: sa2
 *date: 08.06.2016
 */
#include <time.h>                   



#include "anim.h"

/* Ball unit representation type */
typedef struct
{
  sa2UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed; 
  sa2PRIM Pr;  /* Timer speed value*/
} sa2UNIT_CUBE;

/* Cube points */
VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

/* Cube edges */
INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

/* Cube primitive */
sa2PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       sa2UNIT_CUBE *Uni;
 *   - animation context:
 *       sa2ANIM *Ani;
 * RETURNS: None.
 */
static VOID SA2_UnitInit( sa2UNIT_CUBE *Uni, sa2ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 5, Rnd0() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  SA2_RndPrimLoad(&Uni->Pr, "modela\\piggy.g3d");
} /* End of 'SA2_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       sa2UNIT_CUBE *Uni;
 *   - animation context:
 *       sa2ANIM *Ani;
 * RETURNS: None.
 */
static VOID SA2_UnitClose( sa2UNIT_CUBE *Uni, sa2ANIM *Ani )
{
  SA2_RndPrimFree(&Uni->Pr);
} /* End of 'SA2_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       sa2UNIT_CUBE *Uni;
 *   - animation context:
 *       sa2ANIM *Ani;
 * RETURNS: None.
 */
static VOID SA2_UnitResponse( sa2UNIT_CUBE *Uni, sa2ANIM *Ani )
{
 /* if (Ani ->Keys[VK_SPACE])
    SA2_AnimAddUnit(SA2_UnitCreateBall()); */
 /* if (Ani ->Keys"C")
    SA2_AnimAddUnit(SA2_UnitCreateCube());   */

} /* End of 'SA2_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       sa2UNIT_CUBE *Uni;
 *   - animation context:
 *       sa2ANIM *Ani;
 * RETURNS: None.
 */
static VOID SA2_UnitRender( sa2UNIT_CUBE *Uni, sa2ANIM *Ani )
{
  SA2_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.1, 0.1, 0.1)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                                         VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));
  SA2_RndPrimDraw(&Uni->Pr);
} /* End of 'SA2_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (sa2UNIT *) pointer to created unit.
 */
sa2UNIT * SA2_UnitCreateCube( VOID )
{
  sa2UNIT_CUBE *Uni;

  if ((Uni = (sa2UNIT_CUBE *)SA2_AnimUnitCreate(sizeof(sa2UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)SA2_UnitInit;
  Uni->Close = (VOID *)SA2_UnitClose;
  Uni->Response = (VOID *)SA2_UnitResponse;
  Uni->Render = (VOID *)SA2_UnitRender;
  return (sa2UNIT *)Uni;
} /* End of 'SA2_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */
 