/* file name: U_BALL.c
 *progremmer: sa2
 *date: 11.06.2016
 */


#include <time.h>

#include "anim.h"


typedef struct
{
  sa2UNIT; 
  VEC Pos;
  VEC Shift; 
  DBL TimerShift; 
  DBL TimerSpeed;  
  COLORREF Color; 
} sa2UNIT_BALL;

static VOID SA2_UnitInit( sa2UNIT_BALL *Uni, sa2ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB(Rnd0() * 255, Rnd0() * 255, Rnd0() * 255);
} 
static VOID SA2_UnitResponse( sa2UNIT_BALL *Uni, sa2ANIM *Ani )
{
   

  DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;

  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
  
}

static VOID SA2_UnitRender( sa2UNIT_BALL *Uni, sa2ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, p.X - 10, p.Y - 10, p.X + 10, p.Y + 10);
} 
sa2UNIT * SA2_UnitCreateBall( VOID )
{
  sa2UNIT_BALL *Uni;

  if ((Uni = (sa2UNIT_BALL *)SA2_AnimUnitCreate(sizeof(sa2UNIT_BALL))) == NULL)
    return NULL;
  
  Uni->Init = (VOID *)SA2_UnitInit;
  Uni->Response = (VOID *)SA2_UnitResponse;
  Uni->Render = (VOID *)SA2_UnitRender;
  return (sa2UNIT *)Uni;
} 
