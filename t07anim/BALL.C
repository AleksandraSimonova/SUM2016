/* file name: units.c
 *progremmer: sa2
 *date: 11.06.2016
 */
#include "anim.h"
vg4UNIT * VG4_UnitCreateBall( VOID )
{
  vg4UNIT_BALL *Uni;

  if ((Uni = (vg4UNIT_BALL *)VG4_AnimUnitCreate(sizeof(vg4UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)VG4_UnitInit;
  Uni->Response = (VOID *)VG4_UnitResponse;
  Uni->Render = (VOID *)VG4_UnitRender;
  return (vg4UNIT *)Uni;
} 