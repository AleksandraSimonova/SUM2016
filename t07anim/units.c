/* file name: units.c
 *progremmer: sa2
 *date: 11.06.2016
 */
#include "anim.h"



static VOID SA2_UnitInit( sa2UNIT *Uni, sa2ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB(Rnd0() * 255, Rnd0() * 255, Rnd0() * 255);

} 
static VOID SA2_UnitClose( sa2UNIT *Uni, sa2ANIM *Ani )
{
} 
static VOID SA2_UnitResponse( sa2UNIT *Uni, sa2ANIM *Ani )
{
  DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;

  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
} 
 
static VOID SA2_UnitRender( sa2UNIT *Uni, sa2ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, p.X - 10, p.Y - 10, p.X + 10, p.Y + 10);

} 
 
sa2UNIT * SA2_AnimUnitCreate( INT Size )
{
  sa2UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(sa2UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = SA2_UnitInit;
  Uni->Close = SA2_UnitClose;
  Uni->Response = SA2_UnitResponse;
  Uni->Render = SA2_UnitRender;
  return Uni;
} 



