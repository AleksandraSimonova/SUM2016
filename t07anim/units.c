/* file name: units.c
 *progremmer: sa2
 *date: 11.06.2016
 */
#include "anim.h"



static VOID SA2_UnitInit( sa2UNIT *Uni, sa2ANIM *Ani )
{
  

} 
static VOID SA2_UnitClose( sa2UNIT *Uni, sa2ANIM *Ani )
{
} 
static VOID SA2_UnitResponse( sa2UNIT *Uni, sa2ANIM *Ani )
{
  
} 
 
static VOID SA2_UnitRender( sa2UNIT *Uni, sa2ANIM *Ani )
{
  

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



